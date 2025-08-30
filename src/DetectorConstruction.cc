#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// Sensitive Detector 관련 헤더
#include "G4SDManager.hh"
#include "LSSD.hh"

/**
 * @brief 생성자
 */
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

/**
 * @brief 소멸자
 */
DetectorConstruction::~DetectorConstruction()
{}

/**
 * @brief 시뮬레이션의 모든 기하학적 구조를 생성하고 배치하는 메인 함수입니다.
 * @return G4VPhysicalVolume* 생성된 최상위 볼륨(World)의 포인터
 */
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // 1. 물질 정의
  DefineMaterials();

  // 2. World Volume 생성 (모든 것을 담는 가장 큰 공간)
  G4double world_size = 2.0 * m;
  G4Box* solidWorld = new G4Box("SolidWorld", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, fAirMaterial, "LogicWorld");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "PhysWorld", 0, false, 0);

  // 3. 선원 조립체(Source Assembly) 생성
  //    - 어머니-딸 관계를 이용해 복잡한 구조를 만듭니다.
  // 3-1. 에폭시 부분 (선원의 어머니 볼륨)
  G4double epoxy_radius = 15.0 * mm;
  G4double epoxy_hz = 2.5 * mm;
  G4Tubs* solidEpoxy = new G4Tubs("SolidEpoxy", 0, epoxy_radius, epoxy_hz, 0, twopi);
  G4LogicalVolume* logicEpoxy = new G4LogicalVolume(solidEpoxy, fEpoxyMaterial, "LogicEpoxy");

  // 3-2. 60Co 활성 부분 (선원의 딸 볼륨)
  G4double source_radius = 5.0 * mm;
  G4double source_hz = 0.5 * mm;
  G4Tubs* solidSource = new G4Tubs("SolidSource", 0, source_radius, source_hz, 0, twopi);
  G4LogicalVolume* logicSource = new G4LogicalVolume(solidSource, fCo60Material, "LogicSource");
  
  // 3-3. 에폭시(어머니) 안에 Co-60(딸)을 배치합니다.
  new G4PVPlacement(0, G4ThreeVector(), logicSource, "PhysSource", logicEpoxy, false, 0);

  // 4. 검출기 조립체(Detector Assembly) 생성 (어머니 볼륨)
  //    - LS와 PMT 등을 담을 빈 공간(어머니 볼륨)을 먼저 만듭니다.
  //    - 이 방식은 여러 부품을 하나의 단위로 관리하고 배치할 때 매우 유용합니다.
  G4double bottle_outer_radius = 28.0 * mm;
  G4double bottle_height = 100.0 * mm;
  G4double bottle_hz = 0.5 * bottle_height;
  G4double pmt_window_hz = 1.5 * mm;

  G4double detector_assembly_hz = bottle_hz + pmt_window_hz;
  G4Tubs* solidDetectorAssembly = new G4Tubs("SolidDetectorAssembly", 0, bottle_outer_radius, detector_assembly_hz, 0, twopi);
  G4LogicalVolume* logicDetectorAssembly = new G4LogicalVolume(solidDetectorAssembly, fAirMaterial, "LogicDetectorAssembly");
  logicDetectorAssembly->SetVisAttributes(new G4VisAttributes(false)); // 어머니 볼륨은 투명하게

  // 5. 검출기 조립체 내부에 각 부품 배치 (딸 볼륨들)
  // 5-1. 듀란 병 (유리 부분)
  G4double bottle_inner_radius = 26.0 * mm;
  G4Tubs* solidBottle = new G4Tubs("SolidBottle", bottle_inner_radius, bottle_outer_radius, bottle_hz, 0, twopi);
  G4LogicalVolume* logicBottle = new G4LogicalVolume(solidBottle, fGlassMaterial, "LogicBottle");
  new G4PVPlacement(0, G4ThreeVector(0, 0, pmt_window_hz), logicBottle, "PhysBottle", logicDetectorAssembly, false, 0);

  // 5-2. 액체섬광체 (병 내부)
  G4double ls_height = 90.0 * mm;
  G4Tubs* solidLS = new G4Tubs("SolidLS", 0, bottle_inner_radius, 0.5*ls_height, 0, twopi);
  G4LogicalVolume* logicLS = new G4LogicalVolume(solidLS, fLsMaterial, "LogicLS");
  G4double ls_center_z_in_assembly = -bottle_hz + 0.5*ls_height + pmt_window_hz;
  new G4PVPlacement(0, G4ThreeVector(0, 0, ls_center_z_in_assembly), logicLS, "PhysLS", logicDetectorAssembly, false, 0);

  // 5-3. PMT 창
  G4double pmt_radius = 2.0 * 2.54 * cm / 2.0; // 2 inch
  G4Tubs* solidPmtWindow = new G4Tubs("SolidPmtWindow", 0, pmt_radius, pmt_window_hz, 0, twopi);
  G4LogicalVolume* logicPmtWindow = new G4LogicalVolume(solidPmtWindow, fGlassMaterial, "LogicPmtWindow");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -bottle_hz), logicPmtWindow, "PhysPmtWindow", logicDetectorAssembly, false, 0);

  // 6. World Volume 안에 최종 조립체들 배치
  // 6-1. 선원 조립체 배치 (검출기 상단에서 10cm 위에 위치)
  G4double distance = 10.0 * cm;
  G4double detector_top_z = detector_assembly_hz; // 조립체의 최상단 z좌표
  G4ThreeVector sourcePos(0, 0, detector_top_z + distance + epoxy_hz);
  new G4PVPlacement(0, sourcePos, logicEpoxy, "PhysEpoxy", logicWorld, false, 0);

  // 6-2. 검출기 조립체 배치
  G4ThreeVector detector_pos(0, 0, 0);
  new G4PVPlacement(0, detector_pos, logicDetectorAssembly, "PhysDetectorAssembly", logicWorld, false, 0);

  // 7. 시각화 속성 설정
  logicWorld->SetVisAttributes(new G4VisAttributes(false));
  logicEpoxy->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.9, 0.2, 0.3)));
  logicSource->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)));
  logicBottle->SetVisAttributes(new G4VisAttributes(G4Colour(0.8, 0.8, 0.8, 0.3)));
  logicLS->SetVisAttributes(new G4VisAttributes(G4Colour(0.4, 0.9, 0.4, 0.5)));
  logicPmtWindow->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.8, 0.6)));

  return physWorld;
}

/**
 * @brief Sensitive Detector(SD)와 Field를 설정하는 함수입니다.
 * 이 함수는 Construct() 함수가 호출된 후 Geant4 커널에 의해 자동으로 호출됩니다.
 */
void DetectorConstruction::ConstructSDandField()
{
  // 1. "LSSD"라는 이름으로 Sensitive Detector 객체를 생성합니다.
  LSSD* lsSD = new LSSD("LSSD");
  
  // 2. Geant4의 SD 관리자(SDManager)에 생성한 SD를 등록합니다.
  G4SDManager::GetSDMpointer()->AddNewDetector(lsSD);

  // 3. "LogicLS"라는 이름을 가진 논리 볼륨에 등록된 SD를 연결(부착)합니다.
  //    이제부터 "LogicLS" 내부에서 일어나는 모든 스텝은 lsSD에 의해 감지됩니다.
  SetSensitiveDetector("LogicLS", lsSD);
}

/**
 * @brief 시뮬레이션에 사용될 모든 물질을 정의하는 함수입니다.
 */
void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();

  // 원소 정의
  G4Element* H  = new G4Element("Hydrogen", "H",  1.,  1.008*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",  6.,  12.011*g/mole);
  G4Element* N  = new G4Element("Nitrogen", "N",  7.,  14.007*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",  8.,  15.999*g/mole);

  // NIST 데이터베이스에서 표준 물질을 가져옵니다.
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

  // 질량비(mass fraction)를 계산하여 혼합물질을 생성합니다.
  G4double total_mass = (1000.*cm3 * 0.86*g/cm3) + 3.0*g + 0.03*g;
  G4double lab_frac = (1000.*cm3 * 0.86*g/cm3) / total_mass;
  G4double ppo_frac = 3.0*g / total_mass;
  G4double bismsb_frac = 0.03*g / total_mass;

  fLsMaterial = new G4Material("LS", 0.863*g/cm3, 3);
  fLsMaterial->AddMaterial(lab, lab_frac);
  fLsMaterial->AddMaterial(ppo, ppo_frac);
  fLsMaterial->AddMaterial(bismsb, bismsb_frac);
}
