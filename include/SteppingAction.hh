#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

// 클래스 전방 선언
class EventAction;
class G4LogicalVolume;

//______________________________________________________________________________________
// 입자의 모든 스텝(step)마다 호출되어 특정 작업을 수행하는 클래스입니다.
// 주로 특정 볼륨 내에서의 에너지 소실 등을 기록하는 데 사용됩니다.
class SteppingAction : public G4UserSteppingAction
{
public:
  // 생성 시, 데이터를 전달할 EventAction 객체의 포인터를 받습니다.
  SteppingAction(EventAction* eventAction);
  virtual ~SteppingAction();

  // 모든 스텝이 끝날 때마다 Geant4 커널에 의해 호출됩니다.
  virtual void UserSteppingAction(const G4Step*) override;

private:
  // 데이터를 누적시킬 EventAction 객체 포인터
  EventAction* fEventAction;
  
  // 에너지를 측정할 특정 볼륨(Scoring Volume)의 포인터
  // 매 스텝마다 볼륨 이름을 비교하는 것보다 효율적입니다.
  G4LogicalVolume* fScoringVolume;
};

#endif
