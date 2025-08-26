#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;
class G4LogicalVolume;

//______________________________________________________________________________________
class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction* eventAction);
  virtual ~SteppingAction();

  // 모든 스텝이 끝날 때마다 호출됩니다.
  virtual void UserSteppingAction(const G4Step*) override;

private:
  EventAction* fEventAction;
  G4LogicalVolume* fScoringVolume; // 에너지를 측정할 볼륨(액체섬광체)
};

#endif
