#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

//______________________________________________________________________________________
RunAction::RunAction() : G4UserRunAction()
{
  // Analysis Manager를 가져옵니다.
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true); // 멀티스레드에서 Ntuple 병합

  // N-tuple (TTree) 생성
  analysisManager->CreateNtuple("Hits", "Hit data");
  analysisManager->CreateNtupleIColumn("eventID"); // column 0
  analysisManager->CreateNtupleDColumn("edepLS");   // column 1
  // 필요한 다른 변수들 추가 가능 (e.g., edepGlass, edepPMT, ...)
  analysisManager->FinishNtuple();
}

//______________________________________________________________________________________
RunAction::~RunAction()
{}

//______________________________________________________________________________________
void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  // 파일 이름 설정 및 열기
  G4String fileName = "output.root";
  analysisManager->OpenFile(fileName);

  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
}

//______________________________________________________________________________________
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // 파일에 데이터 쓰고 닫기
  analysisManager->Write();
  analysisManager->CloseFile();
}
