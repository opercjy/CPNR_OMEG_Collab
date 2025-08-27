#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

//______________________________________________________________________________________
// 각 이벤트의 시작과 끝에서 필요한 작업을 수행하는 클래스입니다.
// 주로 이벤트별 데이터 초기화 및 저장을 담당합니다.
class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  // 이벤트가 시작되기 직전에 호출됩니다.
  virtual void BeginOfEventAction(const G4Event*) override;

  // 이벤트가 끝난 직후에 호출됩니다.
  virtual void EndOfEventAction(const G4Event*) override;

  // SteppingAction에서 호출하여 스텝별 에너지를 누적시키는 함수입니다.
  void AddEdep(G4double edep);

private:
  // 해당 이벤트 동안 특정 볼륨(여기서는 LS)에 쌓인 총 에너지를 저장하는 변수입니다.
  // BeginOfEventAction에서 0으로 초기화됩니다.
  G4double fEdep;
};

#endif
