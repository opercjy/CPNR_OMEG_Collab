#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

//______________________________________________________________________________________
class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization();
  virtual ~ActionInitialization();

  // Worker 스레드(실제 시뮬레이션 실행)를 위한 Action 클래스들을 생성합니다.
  virtual void Build() const override;

  // Master 스레드(전체 Run 관리)를 위한 Action 클래스들을 생성합니다. (e.g. RunAction)
  virtual void BuildForMaster() const override;
};

#endif
