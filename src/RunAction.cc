// RunAction.cc
// RunAction 클래스의 멤버 함수들을 구현합니다.

#include "RunAction.hh"
#include "G4AnalysisManager.hh" // 데이터 저장을 위한 Analysis Manager 클래스
#include "G4Run.hh"             // Run 정보에 접근하기 위한 클래스

//______________________________________________________________________________________
/**
 * @brief 생성자 (Constructor)
 *
 * RunAction 객체가 생성될 때 호출됩니다.
 * 여기서는 G4AnalysisManager를 이용해 저장할 데이터 구조(N-tuple, 즉 ROOT의 TTree)를
 * 미리 정의합니다. 이 설정은 Run이 시작되기 전에 한 번만 이루어집니다.
 */
RunAction::RunAction() : G4UserRunAction()
{
  // G4AnalysisManager는 데이터 저장을 관리하는 싱글톤 클래스입니다.
  // Instance()를 통해 프로그램 전역에서 동일한 객체에 접근할 수 있습니다.
  auto analysisManager = G4AnalysisManager::Instance();
  
  // 터미널에 출력되는 메시지 레벨을 설정합니다. (1: 기본적인 정보 출력)
  analysisManager->SetVerboseLevel(1);
  
  // 멀티스레드 환경에서 각 스레드가 생성한 ntuple을
  // Run이 끝날 때 하나의 파일로 자동으로 병합하도록 설정합니다.
  analysisManager->SetNtupleMerging(true); 

  // N-tuple (ROOT에서는 TTree에 해당)을 생성합니다.
  // 첫 번째 인자는 ntuple의 이름, 두 번째는 제목입니다.
  analysisManager->CreateNtuple("Hits", "Hit data");
  
  // N-tuple에 들어갈 데이터 컬럼(branch)을 정의합니다.
  // CreateNtupleIColumn은 정수형(Integer) 컬럼을 만듭니다.
  analysisManager->CreateNtupleIColumn("eventID"); // column ID = 0
  
  // CreateNtupleDColumn은 실수형(Double) 컬럼을 만듭니다.
  analysisManager->CreateNtupleDColumn("edepLS");   // column ID = 1
  
  // N-tuple 정의를 마칩니다. 이 함수 호출 이후에는 컬럼을 추가할 수 없습니다.
  analysisManager->FinishNtuple();
}

//______________________________________________________________________________________
/**
 * @brief 소멸자 (Destructor)
 *
 * RunAction 객체가 소멸될 때 호출됩니다.
 * G4AnalysisManager는 Geant4 커널이 관리하므로 여기서 직접 delete할 필요는 없습니다.
 */
RunAction::~RunAction()
{}

//______________________________________________________________________________________
/**
 * @brief BeginOfRunAction
 * @param run 현재 Run에 대한 정보를 담고 있는 객체 포인터
 *
 * Run이 시작될 때 (/run/beamOn 명령 직후) Geant4 커널에 의해 호출됩니다.
 * 여기서는 출력할 ROOT 파일의 이름을 정하고 파일을 엽니다.
 */
void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  // 출력 파일 이름을 설정합니다.
  // 이 이름은 나중에 매크로에서 /analysis/setFileName 명령으로 덮어쓸 수 있습니다.
  G4String fileName = "output.root";
  analysisManager->OpenFile(fileName);

  // 사용자에게 Run이 시작되었음을 알리는 메시지를 터미널에 출력합니다.
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
}

//______________________________________________________________________________________
/**
 * @brief EndOfRunAction
 * @param run 현재 Run에 대한 정보를 담고 있는 객체 포인터
 *
 * Run이 끝날 때 (모든 이벤트 처리가 완료된 후) Geant4 커널에 의해 호출됩니다.
 * 여기서는 지금까지 메모리에 쌓인 데이터를 파일에 쓰고 파일을 닫습니다.
 */
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  // 메모리에 있는 N-tuple 데이터를 파일에 기록합니다.
  analysisManager->Write();
  
  // 파일을 닫습니다.
  analysisManager->CloseFile();
}
