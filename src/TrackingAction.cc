#include "TrackingAction.hh"
#include "G4Track.hh"

//______________________________________________________________________________________
TrackingAction::TrackingAction()
: G4UserTrackingAction()
{}

//______________________________________________________________________________________
TrackingAction::~TrackingAction()
{}

//______________________________________________________________________________________
void TrackingAction::PreUserTrackingAction(const G4Track* /*track*/)
{
  // 트랙이 생성될 때 할 작업 (예: 특정 입자만 추적)
}

//______________________________________________________________________________________
void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/)
{
  // 트랙이 소멸될 때 할 작업
}
