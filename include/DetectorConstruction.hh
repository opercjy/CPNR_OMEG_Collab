#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

// 클래스 전방 선언 (Forward Declaration)
// 헤더 파일에서는 #include 대신 포인터 타입만 알려주는 전방 선언을 사용하면 컴파일 시간을 단축할 수 있습니다.
class G4VPhysicalVolume;
class G4Material;

/**
 * @class DetectorConstruction
 * @brief 시뮬레이션 환경의 모든 물질(Material)과 기하학적 구조(Geometry)를 생성하는 클래스입니다.
 *
 * World Volume부터 시작하여 선원, 검출기 등 모든 요소를 정의하고 배치하는 역할을 담당합니다.
 * 또한, Sensitive Detector를 특정 볼륨에 연결하는 역할도 수행합니다.
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  // Geant4 커널이 호출하여 모든 지오메트리를 생성하는 필수 가상 함수입니다.
  virtual G4VPhysicalVolume* Construct() override;

  // Sensitive Detector(SD)나 자기장(Field)을 설정할 때 사용됩니다.
  virtual void ConstructSDandField() override;

private:
  // 시뮬레이션에 사용될 모든 물질을 정의하는 내부 함수입니다.
  void DefineMaterials();

  // 멤버 변수로 물질 포인터를 선언하여 클래스 내 여러 함수에서 접근할 수 있게 합니다.
  G4Material* fEpoxyMaterial;
  G4Material* fCo60Material;
  G4Material* fGlassMaterial;
  G4Material* fLsMaterial;
  G4Material* fSiliconeMaterial;
  G4Material* fVacuumMaterial;
  G4Material* fAirMaterial;
};

#endif
