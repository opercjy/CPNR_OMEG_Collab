#ifndef PhysicsList_h
#define PhysicsList_h 1

// Geant4의 모듈형 물리 리스트 기본 클래스를 상속받습니다.
#include "G4VModularPhysicsList.hh"

//______________________________________________________________________________________
// 시뮬레이션에 사용될 물리 프로세스들을 정의하고 등록하는 클래스입니다.
class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();
};

#endif
