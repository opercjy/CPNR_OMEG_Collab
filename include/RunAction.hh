#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//______________________________________________________________________________________
class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  // Run이 시작될 때 호출됩니다.
  virtual void BeginOfRunAction(const G4Run*) override;

  // Run이 끝날 때 호출됩니다.
  virtual void EndOfRunAction(const G4Run*) override;
};

#endif
