#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh" // Geant4의 UserRunAction 기본 클래스
#include "globals.hh"       // Geant4 전역 타입 및 상수

//______________________________________________________________________________________
/**
 * @class RunAction
 * @brief 하나의 Run이 시작되고 끝날 때 수행할 작업을 정의하는 클래스입니다.
 *
 * 이 클래스는 주로 데이터 파일(예: ROOT 파일)을 열고,
 * 시뮬레이션이 끝나면 파일에 데이터를 쓰고 닫는 역할을 담당합니다.
 * 멀티스레드 환경에서는 Master 스레드에서만 Begin/EndOfRunAction이 호출되어
 * 전체적인 I/O를 관리합니다.
 */
class RunAction : public G4UserRunAction
{
public:
  // 생성자
  RunAction();
  // 소멸자
  virtual ~RunAction();

  // Geant4 커널이 Run을 시작하기 직전에 호출하는 가상 함수입니다.
  // G4AnalysisManager를 이용해 N-tuple(TTree)을 정의하고 파일을 엽니다.
  virtual void BeginOfRunAction(const G4Run*) override;

  // Geant4 커널이 Run을 마친 직후에 호출하는 가상 함수입니다.
  // G4AnalysisManager를 이용해 파일에 데이터를 쓰고 파일을 닫습니다.
  virtual void EndOfRunAction(const G4Run*) override;
};

#endif
