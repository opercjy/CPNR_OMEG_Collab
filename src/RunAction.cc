#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

/**
 * @brief 생성자: G4AnalysisManager를 이용해 저장할 데이터 구조를 미리 정의합니다.
 * 이 설정은 Run이 시작되기 전에 단 한번만 이루어집니다.
 */
RunAction::RunAction() : G4UserRunAction()
{
  // G4AnalysisManager는 데이터 저장을 관리하는 싱글톤 클래스입니다.
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true); // MT 모드에서 각 스레드의 결과를 병합

  // --- N-tuple ID = 0: Hits TTree ---
  analysisManager->CreateNtuple("Hits", "Hit-by-hit data in LS");
  analysisManager->CreateNtupleIColumn("eventID");      // 0
  analysisManager->CreateNtupleIColumn("trackID");      // 1
  analysisManager->CreateNtupleIColumn("parentID");     // 2
  analysisManager->CreateNtupleSColumn("particleName"); // 3
  analysisManager->CreateNtupleSColumn("processName");  // 4
  analysisManager->CreateNtupleDColumn("x_mm");         // 5
  analysisManager->CreateNtupleDColumn("y_mm");         // 6
  analysisManager->CreateNtupleDColumn("z_mm");         // 7
  analysisManager->CreateNtupleDColumn("time_ns");      // 8
  analysisManager->CreateNtupleDColumn("kineticEnergy_MeV"); // 9
  analysisManager->CreateNtupleDColumn("energyDeposit_MeV"); // 10
  analysisManager->FinishNtuple();

  // --- N-tuple ID = 1: EventSummary TTree ---
  analysisManager->CreateNtuple("EventSummary", "Event-wise summary data");
  analysisManager->CreateNtupleIColumn("eventID");         // 0
  analysisManager->CreateNtupleIColumn("nPrimaries_LS");   // 1: LS에 도달한 1차 입자 수
  analysisManager->CreateNtupleIColumn("nSecondaries_LS"); // 2: LS에 도달한 2차 입자 수
  analysisManager->FinishNtuple();
}

/**
 * @brief 소멸자
 */
RunAction::~RunAction()
{}

/**
 * @brief Run이 시작될 때 (/run/beamOn 명령 직후) 호출됩니다.
 * 출력할 ROOT 파일의 이름을 정하고 파일을 엽니다.
 */
void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  // 기본 파일 이름. 매크로에서 /analysis/setFileName 명령으로 덮어쓸 수 있습니다.
  G4String fileName = "output.root";
  analysisManager->OpenFile(fileName);
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
}

/**
 * @brief Run이 끝날 때 (모든 이벤트 처리 완료 후) 호출됩니다.
 * 지금까지 메모리에 쌓인 데이터를 파일에 쓰고 파일을 닫습니다.
 */
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
