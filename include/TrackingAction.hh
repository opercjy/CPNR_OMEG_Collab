#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

//______________________________________________________________________________________
// 입자 하나하나의 트랙(생성부터 소멸까지의 경로) 단위로 작업을 수행하는 클래스입니다.
class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction();
  virtual ~TrackingAction();

  // 트랙이 시작되기 직전에 호출됩니다.
  virtual void PreUserTrackingAction(const G4Track* track) override;

  // 트랙이 끝난 직후에 호출됩니다.
  virtual void PostUserTrackingAction(const G4Track* track) override;
};

#endif
