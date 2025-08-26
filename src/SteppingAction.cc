SteppingAction.cc#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

//______________________________________________________________________________________
SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(nullptr)
{}

//______________________________________________________________________________________
SteppingAction::~SteppingAction()
{}

//______________________________________________________________________________________
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // 첫 스텝에서 스코어링 볼륨(액체섬광체)을 찾아 저장 (매번 찾는 것 방지)
  if (!fScoringVolume) {
    fScoringVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("LogicLS");
    if (!fScoringVolume) {
        G4Exception("SteppingAction::UserSteppingAction",
                    "ScoringVolumeNotFound", FatalException,
                    "Logical Volume 'LogicLS' not found.");
        return;
    }
  }

  // 현재 스텝이 일어난 볼륨을 가져옴
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()
                               ->GetVolume()->GetLogicalVolume();

  // 현재 볼륨이 스코어링 볼륨이 아니면 아무것도 하지 않음
  if (volume != fScoringVolume) return;

  // 스코어링 볼륨에서 발생한 에너지 소실량을 가져옴
  G4double edep = step->GetTotalEnergyDeposit();
  
  // EventAction에 에너지 값을 전달하여 누적
  fEventAction->AddEdep(edep);
}
