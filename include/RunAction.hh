#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

/**
 * @class RunAction
 * @brief 하나의 Run이 시작되고 끝날 때 수행할 작업을 정의하는 클래스입니다.
 *
 * 주로 데이터 파일(ROOT 파일)을 열고 닫는 역할을 담당하며,
 * 생성자(constructor)에서 G4AnalysisManager를 통해 저장할 데이터 구조(N-tuple, 즉 TTree)를
 * 미리 정의합니다. 이 프로젝트에서는 Hits와 EventSummary, 두 개의 TTree를 정의합니다.
 */
class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  // Run이 시작되기 직전에 호출됩니다.
  virtual void BeginOfRunAction(const G4Run*) override;

  // Run이 마친 직후에 호출됩니다.
  virtual void EndOfRunAction(const G4Run*) override;
};

#endif
