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
void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  // 새 이벤트가 시작될 때마다 에너지 누적 변수 초기화
  fEdep = 0.;
}

//______________________________________________________________________________________
void EventAction::EndOfEventAction(const G4Event* event)
{
  // 이벤트가 끝나면 Analysis Manager를 통해 N-tuple에 데이터 저장
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, event->GetEventID());
  analysisManager->FillNtupleDColumn(1, fEdep);
  analysisManager->AddNtupleRow(); // N-tuple에 한 줄 추가
}

//______________________________________________________________________________________
void EventAction::AddEdep(G4double edep)
{
    fEdep += edep;
}
