// DetectorConstruction.cc
// 시뮬레이션에 필요한 모든 기하학적 구조와 물질을 정의합니다.

#include "DetectorConstruction.hh"

#include "G4Material.hh"          // 물질 정의
#include "G4NistManager.hh"       // NIST 표준 물질 라이브러리
#include "G4Box.hh"               // 사각형 형태
#include "G4Tubs.hh"              // 원통 형태
#include "G4LogicalVolume.hh"     // 논리 볼륨 (형태 + 물질)
#include "G4PVPlacement.hh"       // 물리 볼륨 (논리 볼륨의 배치)
#include "G4SystemOfUnits.hh"     // 물리 단위 (mm, cm, MeV 등)
#include "G4PhysicalConstants.hh" // 물리 상수 (pi 등)
#include "G4VisAttributes.hh"     // 시각화 속성 (색상, 투명도 등)
#include "G4Colour.hh"

// 생성자
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

// 소멸자
DetectorConstruction::~DetectorConstruction()
{}

// Geant4 커널에 의해 호출되어 모든 지오메트리를 생성하는 메인 함수
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // 1. 물질 정의
  DefineMaterials();

  // 2. World Volume 생성 (모든 것을 담는 가장 큰 공간)
  G4double world_size = 2.0 * m;
  G4Box* solidWorld = new G4Box("SolidWorld", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, fAirMaterial, "LogicWorld");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "PhysWorld", 0, false, 0);

  // 3. 선원 조립체 생성 및 배치
  // 3-1. 에폭시 부분 (선원의 어머니 볼륨)
  G4double epoxy_radius = 15.0 * mm;
  G4double epoxy_hz = 2.5 * mm;
  G4Tubs* solidEpoxy = new G4Tubs("SolidEpoxy", 0, epoxy_radius, epoxy_hz, 0, twopi);
  G4LogicalVolume* logicEpoxy = new G4LogicalVolume(solidEpoxy, fEpoxyMaterial, "LogicEpoxy");

  // 3-2. 60Co 활성 부분
  G4double source_radius = 5.0 * mm;
  G4double source_hz = 0.5 * mm;
  G4Tubs* solidSource = new G4Tubs("SolidSource", 0, source_radius, source_hz, 0, twopi);
  G4LogicalVolume* logicSource = new G4LogicalVolume(solidSource, fCo60Material, "LogicSource");
  
  // 3-3. 에폭시 논리 볼륨 안에 60Co 물리 볼륨을 배치
  new G4PVPlacement(0, G4ThreeVector(), logicSource, "PhysSource", logicEpoxy, false, 0);

  // 4. 검출기 조립체 생성
  // 4-1. 듀란 병 (유리 부분)
  G4double bottle_outer_radius = 28.0 * mm;
  G4double bottle_inner_radius = 26.0 * mm;
  G4double bottle_height = 100.0 * mm;
  G4double bottle_hz = 0.5 * bottle_height;
  G4Tubs* solidBottle = new G4Tubs("SolidBottle", bottle_inner_radius, bottle_outer_radius, bottle_hz, 0, twopi);
  G4LogicalVolume* logicBottle = new G4LogicalVolume(solidBottle, fGlassMaterial, "LogicBottle");
  
  // 4-2. 액체섬광체 (병 내부를 채움)
  G4double ls_height = 90.0 * mm;
  G4Tubs* solidLS = new G4Tubs("SolidLS", 0, bottle_inner_radius, 0.5*ls_height, 0, twopi);
  G4LogicalVolume* logicLS = new G4LogicalVolume(solidLS, fLsMaterial, "LogicLS");
  
  // 4-3. PMT 창 (단순화된 모델)
  G4double pmt_radius = 2.0 * 2.54 * cm / 2.0; // 2 inch
  G4double pmt_window_hz = 1.5 * mm;
  G4Tubs* solidPmtWindow = new G4Tubs("SolidPmtWindow", 0, pmt_radius, pmt_window_hz, 0, twopi);
  G4LogicalVolume* logicPmtWindow = new G4LogicalVolume(solidPmtWindow, fGlassMaterial, "LogicPmtWindow");
  
  // 5. World Volume 안에 모든 조립체 배치
  // 5-1. 선원 배치 (병 상단에서 10cm 위에 위치)
  G4double distance = 10.0 * cm;
  G4ThreeVector sourcePos(0, 0, bottle_hz + distance + epoxy_hz);
  new G4PVPlacement(0, sourcePos, logicEpoxy, "PhysEpoxy", logicWorld, false, 0);

  // 5-2. 유리병과 액체섬광체를 각각 World에 배치 (중첩 방지)
  G4ThreeVector detector_center(0, 0, 0);
  new G4PVPlacement(0, detector_center, logicBottle, "PhysBottle", logicWorld, false, 0);
  
  G4ThreeVector ls_pos(0, 0, -bottle_hz + 0.5*ls_height);
  new G4PVPlacement(0, ls_pos, logicLS, "PhysLS", logicWorld, false, 0);

  // 5-3. PMT 창을 병 바로 아래에 배치
  G4ThreeVector pmtWindowPos(0, 0, -bottle_hz - pmt_window_hz);
  new G4PVPlacement(0, pmtWindowPos, "PhysPmtWindow", logicPmtWindow, logicWorld, false, 0);

  // 6. 시각화 속성 설정
  logicWorld->SetVisAttributes(new G4VisAttributes(false)); // World는 투명하게
  logicEpoxy->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.9, 0.2, 0.3))); // 노란색 반투명
  logicSource->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)));   // 파란색
  logicBottle->SetVisAttributes(new G4VisAttributes(G4Colour(0.8, 0.8, 0.8, 0.3)));// 회색 반투명
  logicLS->SetVisAttributes(new G4VisAttributes(G4Colour(0.4, 0.9, 0.4, 0.5)));   // 녹색 반투명
  logicPmtWindow->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.8, 0.6)));// 보라색 반투명

  // 최종적으로 World 물리 볼륨을 반환해야 함
  return physWorld;
}

// 시뮬레이션에 사용될 모든 물질을 정의하는 함수
void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();

  // 원소 정의
  G4Element* H  = new G4Element("Hydrogen", "H",  1.,  1.008*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",  6.,  12.011*g/mole);
  G4Element* N  = new G4Element("Nitrogen", "N",  7.,  14.007*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",  8.,  15.999*g/mole);

  // 공기, 진공, Co, 유리, 실리콘은 NIST 데이터베이스에서 가져옴
  fAirMaterial = nist->FindOrBuildMaterial("G4_AIR");
  fVacuumMaterial = nist->FindOrBuildMaterial("G4_Galactic");
  fCo60Material = nist->FindOrBuildMaterial("G4_Co");
  fGlassMaterial = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  fSiliconeMaterial = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // 에폭시 (C18H20O3) 직접 정의
  fEpoxyMaterial = new G4Material("Epoxy", 1.15*g/cm3, 3);
  fEpoxyMaterial->AddElement(C, 18);
  fEpoxyMaterial->AddElement(H, 20);
  fEpoxyMaterial->AddElement(O, 3);

  // 액체섬광체 (LAB + PPO + bis-MSB) 직접 정의
  G4Material* lab = new G4Material("LAB", 0.86*g/cm3, 2);
  lab->AddElement(C, 18);
  lab->AddElement(H, 30);

  G4Material* ppo = new G4Material("PPO", 1.1*g/cm3, 4);
  ppo->AddElement(C, 15);
  ppo->AddElement(H, 11);
  ppo->AddElement(N, 1);
  ppo->AddElement(O, 1);

  G4Material* bismsb = new G4Material("bisMSB", 1.05*g/cm3, 2);
  bismsb->AddElement(C, 24);
  bismsb->AddElement(H, 22);

  // 질량비 계산 (1L 기준)
  G4double total_mass = (1000.*cm3 * 0.86*g/cm3) + 3.0*g + 0.03*g;
  G4double lab_frac = (1000.*cm3 * 0.86*g/cm3) / total_mass;
  G4double ppo_frac = 3.0*g / total_mass;
  G4double bismsb_frac = 0.03*g / total_mass;

  // 최종 액체섬광체 물질 생성
  fLsMaterial = new G4Material("LS", 0.863*g/cm3, 3);
  fLsMaterial->AddMaterial(lab, lab_frac);
  fLsMaterial->AddMaterial(ppo, ppo_frac);
  fLsMaterial->AddMaterial(bismsb, bismsb_frac);
}

// Sensitive Detector 등을 설정하는 함수 (현재는 사용하지 않음, 광학 모델 사용시 활성화)
void DetectorConstruction::ConstructSDandField()
{}
