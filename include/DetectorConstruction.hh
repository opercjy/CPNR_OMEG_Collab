#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh" // Geant4의 DetectorConstruction 기본 클래스
#include "globals.hh"

// 클래스 전방 선언 (Forward Declaration)
// 헤더 파일에서는 전체 클래스 정의(#include) 대신 포인터 타입만 알려주는 전방 선언을 사용하면
// 컴파일 시간을 단축하는 데 도움이 됩니다.
class G4VPhysicalVolume;
class G4Material;

//______________________________________________________________________________________
/**
 * @class DetectorConstruction
 * @brief 시뮬레이션 환경의 모든 물질과 기하학적 구조를 생성하는 클래스입니다.
 *
 * World Volume부터 시작하여 선원, 검출기 등 모든 요소를 정의하고 배치하는
 * 역할을 담당합니다. 이 클래스에서 정의된 지오메트리는 Geant4 커널에 의해
 * 시뮬레이션 시작 전에 메모리에 구성됩니다.
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  // 생성자
  DetectorConstruction();
  // 소멸자
  virtual ~DetectorConstruction();

  // Geant4 커널이 호출하여 모든 지오메트리를 생성하는 필수 가상 함수입니다.
  // 이 함수는 World Volume의 물리 볼륨 포인터를 반환해야 합니다.
  virtual G4VPhysicalVolume* Construct() override;

  // 민감 검출기(Sensitive Detector)나 자기장(Field)을 설정할 때 사용됩니다.
  // 이번 명세에서는 사용하지 않지만, 향후 확장을 위해 기본 틀을 유지합니다.
  virtual void ConstructSDandField() override;

private:
  // 시뮬레이션에 사용될 모든 물질을 정의하는 내부 함수입니다.
  void DefineMaterials();

  // 멤버 변수로 물질 포인터를 선언합니다.
  // 이렇게 하면 Construct() 함수 내에서 물질을 정의하고 사용하는 것뿐만 아니라,
  // 다른 함수에서도 물질 정보에 접근할 수 있게 됩니다.
  G4Material* fEpoxyMaterial;
  G4Material* fCo60Material;
  G4Material* fGlassMaterial;
  G4Material* fLsMaterial;
  G4Material* fSiliconeMaterial;
  G4Material* fVacuumMaterial;
  G4Material* fAirMaterial;
};

#endif
