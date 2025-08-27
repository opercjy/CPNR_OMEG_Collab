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
  // 이 함수는 트랙이 생성될 때 호출됩니다.
  // 예를 들어, 특정 입자(e.g., 광전자)가 생성되는 것을 추적하는 코드를 여기에 넣을 수 있습니다.
}

//______________________________________________________________________________________
void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/)
{
  // 이 함수는 트랙이 소멸(에너지를 다 잃거나, 다른 입자로 변환되거나, 지오메트리를 벗어남)될 때 호출됩니다.
}
