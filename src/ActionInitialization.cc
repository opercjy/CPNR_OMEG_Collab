#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

//______________________________________________________________________________________
ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

//______________________________________________________________________________________
ActionInitialization::~ActionInitialization()
{}

//______________________________________________________________________________________
void ActionInitialization::BuildForMaster() const
{
  // Master 스레드에서는 RunAction만 필요합니다.
  SetUserAction(new RunAction());
}

//______________________________________________________________________________________
void ActionInitialization::Build() const
{
  // Worker 스레드에서는 모든 Action 클래스가 필요합니다.
  SetUserAction(new PrimaryGeneratorAction());
  
  RunAction* runAction = new RunAction();
  SetUserAction(runAction);
  
  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);
  
  SetUserAction(new SteppingAction(eventAction));
  SetUserAction(new TrackingAction());
}
