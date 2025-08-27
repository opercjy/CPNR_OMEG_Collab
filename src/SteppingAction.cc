#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh" // 볼륨 정보를 가져오기 위해 필요할 수 있음

#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh" // 모든 논리 볼륨을 관리하는 저장소

//______________________________________________________________________________________
SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(nullptr) // 생성자에서 포인터를 nullptr로 초기화
{}

//______________________________________________________________________________________
SteppingAction::~SteppingAction()
{}

//______________________________________________________________________________________
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // 1. 첫 스텝에서 스코어링 볼륨(액체섬광체)을 찾아 멤버 변수에 저장합니다.
  // 이렇게 하면 매 스텝마다 볼륨을 찾는 비효율을 방지할 수 있습니다.
  if (!fScoringVolume) {
    fScoringVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("LogicLS");
    if (!fScoringVolume) { // 만약 "LogicLS"를 못찾으면 프로그램을 중단시켜 오류를 알림
        G4Exception("SteppingAction::UserSteppingAction",
                    "ScoringVolumeNotFound", FatalException,
                    "Logical Volume 'LogicLS' not found.");
        return;
    }
  }

  // 2. 현재 스텝이 일어난 볼륨의 논리 볼륨을 가져옵니다.
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()
                               ->GetVolume()->GetLogicalVolume();

  // 3. 현재 볼륨이 우리가 관심 있는 스코어링 볼륨이 아니면, 아무것도 하지 않고 함수를 종료합니다.
  if (volume != fScoringVolume) return;

  // 4. 스코어링 볼륨 안에서 발생한 에너지 소실량을 가져옵니다.
  G4double edep = step->GetTotalEnergyDeposit();
  
  // 5. EventAction의 AddEdep 함수를 호출하여 에너지를 전달하고 누적시킵니다.
  fEventAction->AddEdep(edep);
}
