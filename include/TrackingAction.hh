#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

/**
 * @class TrackingAction
 * @brief 입자 하나하나의 트랙(생성부터 소멸까지의 경로) 단위로 작업을 수행하는 클래스입니다.
 *
 * 현재 프로젝트에서는 사용하고 있지 않지만, 향후 특정 입자의 추적 시작/종료 시점에
 * 필요한 로직을 구현하기 위한 표준적인 틀(placeholder)로 남겨두었습니다.
 */
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
