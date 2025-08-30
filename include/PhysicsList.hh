#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
// 이제 경로를 지정해 줄 것이므로, 컴파일러가 이 파일을 찾을 수 있습니다.
// #include "Shielding.hh"

/**
 * @class PhysicsList
 * @brief 시뮬레이션에 사용될 모든 물리 프로세스를 정의하고 등록하는 클래스입니다.
 *
 * G4VModularPhysicsList를 상속받아, 필요한 물리 모듈(전자기, 방사성 붕괴, 강입자 등)을
 * 독립적으로 조합하여 사용합니다. 이 방식은 Geant4에서 권장하는 현대적인 방식으로,
 * 특정 표준 물리 리스트(예: Shielding)에 종속될 때 발생하는 문제를 해결합니다.
 */
class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();
};

#endif
