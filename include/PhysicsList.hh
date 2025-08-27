#ifndef PhysicsList_h
#define PhysicsList_h 1

// Shielding 클래스의 기반이 되는 헤더를 포함해주는 것이 안정적입니다.
#include "G4VModularPhysicsList.hh"
// 이제 경로를 지정해 줄 것이므로, 컴파일러가 이 파일을 찾을 수 있습니다.
#include "Shielding.hh"

// G4Shielding이 아닌 Shielding 클래스를 상속받습니다.
class PhysicsList : public Shielding
{
public:
  PhysicsList();
  virtual ~PhysicsList();
};

#endif
