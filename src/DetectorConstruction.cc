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

//______________________________________________________________________________________
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
}

//______________________________________________________________________________________
DetectorConstruction::~DetectorConstruction()
{
}

//______________________________________________________________________________________
void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();

  // 원소 정의
  G4double a, z;
  G4Element* H  = new G4Element("Hydrogen", "H",  z=1.,  a=1.008*g/mole);
  G4Element* C  = new G4Element("Carbon",   "C",  z=6.,  a=12.011*g/mole);
  G4Element* N  = new G4Element("Nitrogen", "N",  z=7.,  a=14.007*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O",  z=8.,  a=15.999*g/mole);

  // 1. 공기 (Air)
  fAirMaterial = nist->FindOrBuildMaterial("G4_AIR");

  // 2. 진공 (Vacuum)
  fVacuumMaterial = nist->FindOrBuildMaterial("G4_Galactic");

  // 3. 에폭시 (Epoxy Resin, C18H20O3)
  fEpoxyMaterial = new G4Material("Epoxy", 1.15*g/cm3, 3);
  fEpoxyMaterial->AddElement(C, 18);
  fEpoxyMaterial->AddElement(H, 20);
  fEpoxyMaterial->AddElement(O, 3);

  // 4. 코발트 (60Co)
  fCo60Material = nist->FindOrBuildMaterial("G4_Co");

  // 5. 붕규산 유리 (Borosilicate Glass)
  fGlassMaterial = nist->FindOrBuildMaterial("G4_Pyrex_Glass"); // Pyrex is a common type

  // 6. 실리콘 광학 커플러 (Silicone)
  fSiliconeMaterial = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); // Approximation

  // 7. 액체섬광체 (LAB + PPO + bis-MSB)
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

  G4double total_mass = (1000.*cm3 * 0.86*g/cm3) + 3.0*g + 0.03*g;
  G4double lab_frac = (1000.*cm3 * 0.86*g/cm3) / total_mass;
  G4double ppo_frac = 3.0*g / total_mass;
  G4double bismsb_frac = 0.03*g / total_mass;

  fLsMaterial = new G4Material("LS", 0.863*g/cm3, 3);
  fLsMaterial->AddMaterial(lab, lab_frac);
  fLsMaterial->AddMaterial(ppo, ppo_frac);
  fLsMaterial->AddMaterial(bismsb, bismsb_frac);
}

//______________________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // 물질 정의
  DefineMaterials();

  //---- World Volume ----//
  G4double world_size = 2.0 * m;
  G4Box* solidWorld = new G4Box("SolidWorld", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, fAirMaterial, "LogicWorld");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "PhysWorld", 0, false, 0);

  //---- Source Assembly ----//
  G4double epoxy_radius = 15.0 * mm;
  G4double epoxy_hz = 2.5 * mm;
  G4Tubs* solidEpoxy = new G4Tubs("SolidEpoxy", 0, epoxy_radius, epoxy_hz, 0, twopi);
  G4LogicalVolume* logicEpoxy = new G4LogicalVolume(solidEpoxy, fEpoxyMaterial, "LogicEpoxy");

  G4double source_radius = 5.0 * mm;
  G4double source_hz = 0.5 * mm;
  G4Tubs* solidSource = new G4Tubs("SolidSource", 0, source_radius, source_hz, 0, twopi);
  G4LogicalVolume* logicSource = new G4LogicalVolume(solidSource, fCo60Material, "LogicSource");

  // 에폭시 안에 소스 배치
  new G4PVPlacement(0, G4ThreeVector(), logicSource, "PhysSource", logicEpoxy, false, 0);

  //---- Detector Assembly (Duran Bottle + LS + PMT) ----//
  // 100ml Duran Bottle (GL45) approx. dimensions
  G4double bottle_outer_radius = 28.0 * mm;
  G4double bottle_inner_radius = 26.0 * mm;
  G4double bottle_height = 100.0 * mm;
  G4double bottle_hz = 0.5 * bottle_height;

  // Bottle Body
  G4Tubs* solidBottle = new G4Tubs("SolidBottle", bottle_inner_radius, bottle_outer_radius, bottle_hz, 0, twopi);
  G4LogicalVolume* logicBottle = new G4LogicalVolume(solidBottle, fGlassMaterial, "LogicBottle");
  
  // Liquid Scintillator inside the bottle
  G4double ls_height = 80.0 * mm; // Approx for 100ml
  G4Tubs* solidLS = new G4Tubs("SolidLS", 0, bottle_inner_radius, 0.5*ls_height, 0, twopi);
  G4LogicalVolume* logicLS = new G4LogicalVolume(solidLS, fLsMaterial, "LogicLS");
  new G4PVPlacement(0, G4ThreeVector(0,0, -bottle_hz + 0.5*ls_height), logicLS, "PhysLS", logicBottle, false, 0);
  
  // PMT Assembly (simplified)
  G4double pmt_radius = 2.0 * 2.54 * cm / 2.0; // 2 inch
  G4double pmt_window_hz = 1.5 * mm;
  G4Tubs* solidPmtWindow = new G4Tubs("SolidPmtWindow", 0, pmt_radius, pmt_window_hz, 0, twopi);
  G4LogicalVolume* logicPmtWindow = new G4LogicalVolume(solidPmtWindow, fGlassMaterial, "LogicPmtWindow");
  
  //---- Placement in World ----//
  // Source is placed 10cm above the bottle top
  G4double distance = 10.0 * cm;
  G4ThreeVector sourcePos(0, 0, bottle_hz + distance + epoxy_hz);
  new G4PVPlacement(0, sourcePos, logicEpoxy, "PhysEpoxy", logicWorld, false, 0);

  // Bottle is at the origin
  new G4PVPlacement(0, G4ThreeVector(), logicBottle, "PhysBottle", logicWorld, false, 0);

  // PMT is placed just below the bottle
  G4ThreeVector pmtWindowPos(0, 0, -bottle_hz - pmt_window_hz);
  new G4PVPlacement(0, pmtWindowPos, logicPmtWindow, "PhysPmtWindow", logicWorld, false, 0);


  //---- Visualization Attributes ----//
  logicWorld->SetVisAttributes(new G4VisAttributes(false));
  logicEpoxy->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.9, 0.2, 0.3))); // Yellowish transparent
  logicSource->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 0.0, 1.0))); // Blue
  logicBottle->SetVisAttributes(new G4VisAttributes(G4Colour(0.8, 0.8, 0.8, 0.3))); // Grey transparent
  logicLS->SetVisAttributes(new G4VisAttributes(G4Colour(0.4, 0.9, 0.4, 0.5))); // Greenish transparent
  logicPmtWindow->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.8, 0.6))); // Purplish

  // 항상 physWorld를 반환해야 함
  return physWorld;
}

//______________________________________________________________________________________
void DetectorConstruction::ConstructSDandField()
{
  // Sensitive Detector, Magnetic Field 등 설정 (현재는 비어있음)
}
