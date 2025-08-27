#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

//______________________________________________________________________________________
class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  // 이벤트가 시작될 때 호출됩니다.
  virtual void BeginOfEventAction(const G4Event*) override;

  // 이벤트가 끝날 때 호출됩니다.
  virtual void EndOfEventAction(const G4Event*) override;

  // SteppingAction에서 호출하여 에너지를 누적시키는 함수
  void AddEdep(G4double edep);

private:
  // 해당 이벤트 동안 섬광체에 쌓인 에너지를 저장하는 변수
  G4double fEdep;
};

#endif
