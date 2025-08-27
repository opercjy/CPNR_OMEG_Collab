// CPNR_OMEG_colab.cc (Final Version with Comments)
// 이 파일은 시뮬레이션의 시작점(entry point)이며, 전체 실행 흐름을 제어합니다.

// Geant4의 핵심 관리 클래스들을 포함합니다.
#include "G4RunManagerFactory.hh" // RunManager 생성
#include "G4UImanager.hh"         // UI 명령어 처리

// GUI 및 시각화 관련 클래스들을 포함합니다.
#include "G4VisExecutive.hh"      // 시각화 관리자
#include "G4UIExecutive.hh"       // UI 세션 관리

// 우리가 직접 작성한 필수 사용자 클래스들의 헤더 파일을 포함합니다.
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
  // --- 1. UI 세션 감지 ---
  // 프로그램 실행 시 인자(argument)가 없으면(argc=1) 인터랙티브 모드로 판단하고,
  // Qt 기반의 UI 세션 객체를 생성합니다.
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // --- 2. RunManager 생성 ---
  // 멀티스레딩을 지원하는 기본 RunManager를 생성합니다.
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // --- 3. 시각화 관리자 초기화 ---
  // 시각화 기능을 사용하기 위해 G4VisExecutive 객체를 생성하고 초기화합니다.
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // --- 4. 필수 사용자 클래스 등록 ---
  // Geant4 커널에 우리가 만든 '설계도'들을 알려줍니다.
  runManager->SetUserInitialization(new DetectorConstruction()); // 지오메트리 설정
  runManager->SetUserInitialization(new PhysicsList());          // 물리 법칙 설정
  runManager->SetUserInitialization(new ActionInitialization()); // 데이터 처리 방식 설정

  // --- 5. Geant4 커널 초기화 ---
  // 등록된 클래스들을 바탕으로 Geant4 커널(지오메트리, 물리 테이블 등)을 초기화합니다.
  runManager->Initialize();

  // --- 6. UI 관리자 포인터 획득 ---
  // 매크로 파일 실행 등 UI 명령어를 처리하는 객체의 포인터를 가져옵니다.
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // --- 7. 실행 모드 분기 ---
  if (ui) {
    // ## 인터랙티브 모드 ## (ui 객체가 생성되었을 경우)
    // 초기 시각화 설정을 담은 init_vis.mac 파일을 실행합니다.
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    // 사용자가 GUI 창을 닫을 때까지 프로그램이 대기하며 상호작용하는 세션을 시작합니다.
    ui->SessionStart();
    // 세션이 끝나면 ui 객체를 삭제합니다.
    delete ui;
  }
  else {
    // ## 배치 모드 ## (프로그램 실행 시 인자가 있었을 경우)
    // 첫 번째 인자(argv[1])를 매크로 파일 경로로 간주합니다.
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    // 해당 매크로 파일을 실행합니다.
    UImanager->ApplyCommand(command + fileName);
  }

  // --- 8. 종료 처리 ---
  // 시뮬레이션이 모두 끝나면 생성했던 객체들을 메모리에서 해제합니다.
  delete visManager;
  delete runManager;

  return 0;
}
