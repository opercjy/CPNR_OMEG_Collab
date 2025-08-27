#include "PhysicsList.hh"

// 필요한 물리 모델들의 헤더 파일을 포함합니다.
#include "G4DecayPhysics.hh"             // 입자 붕괴
#include "G4RadioactiveDecayPhysics.hh" // 방사성 붕괴 (핵심)
#include "G4EmStandardPhysics_option4.hh" // 전자기 상호작용 (가장 정밀한 표준 모델)
#include "G4HadronPhysicsFTFP_BERT.hh"  // 강입자 물리 (고에너지)
#include "G4HadronElasticPhysics.hh"    // 강입자 탄성 산란
#include "G4IonPhysics.hh"              // 이온 물리
#include "G4StoppingPhysics.hh"         // 저에너지 하전 입자 물리

//______________________________________________________________________________________
// 생성자에서 필요한 물리 모델 객체를 생성하고 등록(RegisterPhysics)합니다.
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // 1. 입자 붕괴 물리
  RegisterPhysics(new G4DecayPhysics());

  // 2. 방사성 붕괴 물리 (60Co 붕괴 시뮬레이션을 위해 필수)
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // 3. 전자기 물리 (광전효과, 컴프턴 산란, 쌍생성 등)
  RegisterPhysics(new G4EmStandardPhysics_option4());

  // 4. 강입자 물리 (FTFP_BERT 모델 기반)
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4IonPhysics());
  RegisterPhysics(new G4StoppingPhysics());
}

//______________________________________________________________________________________
PhysicsList::~PhysicsList()
{}
