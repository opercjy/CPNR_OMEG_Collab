// Geant4 코어 헤더
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

// 시각화 및 UI 관련 헤더
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// 사용자가 정의한 필수 클래스 헤더
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

// C++ 표준 라이브러리
#include <iostream>

//______________________________________________________________________________________
int main(int argc, char** argv)
{
  // 환영 메시지 출력
  G4cout << "========================================================" << G4endl;
  G4cout << "    CPNR-OMEG-colab: Geant4 Simulation" << G4endl;
  G4cout << "========================================================" << G4endl;

  // 1. 기본 RunManager 생성
  // 멀티스레딩을 지원하는 기본 RunManager를 생성합니다.
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  G4cout << "--> RunManager 생성 완료." << G4endl;

  // 2. 필수 사용자 클래스 등록
  // 시뮬레이션에 필요한 3대 필수 요소를 RunManager에 연결합니다.
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());
  G4cout << "--> 사용자 초기화 클래스 등록 완료." << G4endl;

  // RunManager 초기화 (기하학 구조, 물리 모델 등을 메모리에 로드)
  runManager->Initialize();
  G4cout << "--> RunManager 초기화 완료." << G4endl;

  // 3. 시각화 관리자 초기화
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4cout << "--> 시각화 관리자 초기화 완료." << G4endl;

  // 4. UI 관리자 포인터 가져오기
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // 5. 실행 모드 분기 처리
  if (argc != 1) {
    // ## 배치 모드 (Batch Mode) ##
    // 프로그램 실행 시 매크로 파일 경로를 인자로 받은 경우
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    G4cout << "--> 배치 모드로 실행: " << fileName << " 매크로를 실행합니다." << G4endl;
    UImanager->ApplyCommand(command + fileName);
  }
  else {
    // ## 인터랙티브 모드 (Interactive Mode) ##
    // 인자 없이 실행된 경우 (CMake에서 Qt 옵션을 켰을 때만 활성화됨)
    #ifdef G4UI_USE
      G4cout << "--> 인터랙티브 모드로 실행 (Qt GUI)." << G4endl;
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);

      // 초기 시각화 매크로 파일 실행 ( geometry, vis attributes 등)
      // 이 파일이 없으면 경고만 뜨고 정상 실행됩니다.
      UImanager->ApplyCommand("/control/execute init_vis.mac");

      // UI 세션 시작 (사용자가 GUI를 닫을 때까지 대기)
      ui->SessionStart();
      delete ui;
    #else
      G4cout << "오류: 이 실행 파일은 GUI 없이 빌드되었습니다." << G4endl;
      G4cout << "매크로 파일을 인자로 전달하여 배치 모드로 실행하십시오." << G4endl;
      G4cout << "예: ./CPNR_OMEG_colab run.mac" << G4endl;
    #endif
  }

  // 6. 종료 처리
  // 사용된 객체들의 메모리를 해제합니다.
  delete visManager;
  delete runManager;

  G4cout << "--> 시뮬레이션이 성공적으로 종료되었습니다." << G4endl;

  return 0;
}
//______________________________________________________________________________________
