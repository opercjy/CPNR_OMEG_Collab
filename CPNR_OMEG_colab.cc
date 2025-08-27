// CPNR_OMEG_colab.cc (Final Corrected Version)
// 시뮬레이션의 시작점(entry point)이며, 전체 실행 흐름을 제어합니다.

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
  // 1. UI 세션을 가장 먼저 감지합니다.
  G4UIExecutive* ui = nullptr;
  if (argc == 1) { // 인자가 없으면 GUI 모드로 판단
    ui = new G4UIExecutive(argc, argv);
  }

  // 2. 기본 RunManager를 생성합니다.
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // 3. 시각화 관리자를 생성하고 초기화합니다.
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // 4. 필수 사용자 클래스를 등록합니다.
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());

  // 5. Geant4 커널을 초기화합니다.
  // 이 단계에서 지오메트리가 메모리에 생성됩니다.
  runManager->Initialize();

  // 6. UI 관리자 포인터를 가져옵니다.
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // 7. 실행 모드를 분기합니다.
  if (ui) {
    // ## 인터랙티브 모드 ##
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
    // ## 배치 모드 ##
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  // 8. 종료 처리
  delete visManager;
  delete runManager;

  return 0;
}
