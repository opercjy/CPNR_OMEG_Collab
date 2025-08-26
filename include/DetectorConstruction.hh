#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

//______________________________________________________________________________________
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  // G4VUserDetectorConstruction의 필수 가상 함수
  // 이 함수에서 모든 지오메트리를 생성합니다.
  virtual G4VPhysicalVolume* Construct() override;

  // 이 함수는 Sensitive Detector(민감 검출기)나 필드를 설정할 때 사용됩니다.
  // 이번 명세에서는 사용하지 않지만, 기본 틀을 위해 남겨둡니다.
  virtual void ConstructSDandField() override;

private:
  // 물질 정의 함수
  void DefineMaterials();

  // 각 지오메토리 컴포넌트를 생성하는 함수
  G4VPhysicalVolume* DefineWorld();
  G4VPhysicalVolume* DefineSourceAssembly(G4LogicalVolume* worldLog);
  G4VPhysicalVolume* DefineDetectorAssembly(G4LogicalVolume* worldLog);

  // 여기에 물질 포인터를 멤버 변수로 선언하여
  // Construct() 함수 내에서 재정의하는 것을 방지할 수 있습니다.
  G4Material* fEpoxyMaterial;
  G4Material* fCo60Material;
  G4Material* fGlassMaterial;
  G4Material* fLsMaterial;
  G4Material* fSiliconeMaterial;
  G4Material* fVacuumMaterial;
  G4Material* fAirMaterial;
};

#endif
