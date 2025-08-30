#include "SteppingAction.hh"
#include "G4Step.hh"

/**
 * @brief 생성자
 */
SteppingAction::SteppingAction() : G4UserSteppingAction() {}

/**
 * @brief 소멸자
 */
SteppingAction::~SteppingAction() {}

/**
 * @brief 입자가 한 스텝을 진행할 때마다 호출됩니다.
 *
 * 이 프로젝트에서는 데이터 수집 로직을 더 효율적인 LSSD (Sensitive Detector)로
 * 이전했기 때문에, 이 함수의 내용은 의도적으로 비워두었습니다.
 */
void SteppingAction::UserSteppingAction(const G4Step* /*step*/) {}
