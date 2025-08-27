#include "PhysicsList.hh"
#include "G4RadioactiveDecayPhysics.hh"

// G4Shielding()이 아닌 Shielding() 생성자를 호출합니다.
PhysicsList::PhysicsList() : Shielding()
{
  // Shielding 물리 리스트에 방사성 붕괴 모델을 추가로 등록합니다.
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}
