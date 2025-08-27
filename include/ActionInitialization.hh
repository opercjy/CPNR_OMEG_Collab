#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

//______________________________________________________________________________________
// 모든 User Action 클래스들을 생성하고 Geant4 RunManager에 등록하는 클래스입니다.
// 멀티스레드 모드에서 Master 스레드와 Worker 스레드에 각각 필요한 Action들을
// 적절히 배분하는 역할을 합니다.
class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization();
  virtual ~ActionInitialization();

  // Worker 스레드(실제 시뮬레이션 이벤트 처리)를 위한 Action 클래스들을 생성합니다.
  virtual void Build() const override;

  // Master 스레드(전체 Run 관리, 데이터 병합 등)를 위한 Action 클래스를 생성합니다.
  virtual void BuildForMaster() const override;
};

#endif
