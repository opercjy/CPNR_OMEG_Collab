#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4SystemOfUnits.hh"

/**
 * @brief 생성자: 필요한 물리 프로세스 모듈들을 등록합니다.
 *
 * G4VModularPhysicsList를 상속받아, 필요한 물리 모듈을 '부품'처럼 조립합니다.
 * 이 방식은 특정 물리 리스트(예: Shielding)에 종속되지 않아 제어가 명확하고
 * UI 명령어 충돌과 같은 예기치 않은 문제를 방지합니다.
 */
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // 1. 표준 전자기 물리 (감마, 전자 상호작용에 필수)
  RegisterPhysics(new G4EmStandardPhysics());

  // 2. 일반 입자 붕괴 물리 (예: 파이온, 뮤온 등)
  RegisterPhysics(new G4DecayPhysics());

  // 3. 방사성 동위원소 붕괴 물리
  // 이것이 /process/had/rdm/ 명령어들을 활성화하는 핵심입니다.
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // 4. 강입자(Hadron) 물리 (고정밀 중성자 모델을 포함)
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP());
  RegisterPhysics(new G4HadronElasticPhysicsHP());
  
  // 5. 이온(Ion) 및 정지(Stopping) 관련 물리
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());
  
  // 2차 입자 생성을 위한 기준 거리(Production Cut)를 1mm로 설정
  SetDefaultCutValue(1.0*mm);
}

PhysicsList::~PhysicsList()
{}
