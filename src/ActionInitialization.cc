#include "ActionInitialization.hh"

// 생성할 모든 Action 클래스들의 헤더 파일을 포함합니다.
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

//______________________________________________________________________________________
ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

//______________________________________________________________________________________
ActionInitialization::~ActionInitialization()
{}

//______________________________________________________________________________________
// 이 함수는 Master 스레드에서 단 한번만 호출됩니다.
// 전체 Run의 시작과 끝을 관리하는 RunAction만 생성하여 등록합니다.
void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction());
}

//______________________________________________________________________________________
// 이 함수는 각 Worker 스레드마다 호출됩니다.
// 각 스레드는 독립적으로 이벤트를 처리하므로, 모든 Action 클래스가 필요합니다.
void ActionInitialization::Build() const
{
  // 1. 초기 입자 생성을 담당하는 클래스
  SetUserAction(new PrimaryGeneratorAction());
  
  // 2. Run의 시작/끝, 파일 I/O 등을 담당하는 클래스
  RunAction* runAction = new RunAction();
  SetUserAction(runAction);
  
  // 3. 개별 이벤트의 시작/끝, 데이터 취합 등을 담당하는 클래스
  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);
  
  // 4. 입자의 스텝마다 데이터 수집을 담당하는 클래스
  // EventAction에 데이터를 전달해야 하므로, 생성 시 eventAction의 포인터를 넘겨줍니다.
  SetUserAction(new SteppingAction(eventAction));

  // 5. 입자의 트랙 단위 정보 처리를 담당하는 클래스
  SetUserAction(new TrackingAction());
}
