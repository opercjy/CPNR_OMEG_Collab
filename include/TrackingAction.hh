#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

//______________________________________________________________________________________
class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction();
  virtual ~TrackingAction();

  // 트랙이 시작되기 전에 호출됩니다.
  virtual void PreUserTrackingAction(const G4Track* track) override;

  // 트랙이 끝난 후에 호출됩니다.
  virtual void PostUserTrackingAction(const G4Track* track) override;
};

#endif
