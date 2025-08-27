#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

//______________________________________________________________________________________
EventAction::EventAction()
: G4UserEventAction(), fEdep(0.)
{}

//______________________________________________________________________________________
EventAction::~EventAction()
{}

//______________________________________________________________________________________
// 새 이벤트가 시작될 때마다 에너지 누적 변수를 0으로 초기화합니다.
// 이렇게 하지 않으면 이전 이벤트의 에너지가 계속 더해지게 됩니다.
void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  fEdep = 0.;
}

//______________________________________________________________________________________
// 이벤트가 끝나면, 누적된 에너지(fEdep)를 G4AnalysisManager를 통해 N-tuple에 저장합니다.
void EventAction::EndOfEventAction(const G4Event* event)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // N-tuple의 각 컬럼에 값을 채워넣습니다. 컬럼 ID는 RunAction에서 정의한 순서입니다.
  analysisManager->FillNtupleIColumn(0, event->GetEventID()); // 0번 컬럼: eventID
  analysisManager->FillNtupleDColumn(1, fEdep);               // 1번 컬럼: edepLS
  
  // 채워진 한 줄의 데이터를 N-tuple에 추가합니다.
  analysisManager->AddNtupleRow();
}

//______________________________________________________________________________________
// 이 함수는 SteppingAction에서 호출됩니다.
// 스텝에서 발생한 에너지를 멤버 변수 fEdep에 더합니다.
void EventAction::AddEdep(G4double edep)
{
    fEdep += edep;
}
