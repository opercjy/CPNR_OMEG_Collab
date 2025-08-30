#include "TrackingAction.hh"
#include "G4Track.hh"

/**
 * @brief 생성자
 */
TrackingAction::TrackingAction() : G4UserTrackingAction() {}

/**
 * @brief 소멸자
 */
TrackingAction::~TrackingAction() {}

/**
 * @brief 트랙이 생성될 때 호출됩니다.
 *
 * 현재는 비어있지만, 향후 특정 입자(예: 광전자)의 생성을 추적하는 등의
 * 로직을 여기에 추가할 수 있습니다.
 */
void TrackingAction::PreUserTrackingAction(const G4Track* /*track*/) {}

/**
 * @brief 트랙이 소멸될 때 호출됩니다.
 */
void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/) {}
