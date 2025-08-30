#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/**
 * @class EventAction
 * @brief 각 이벤트(Event)의 시작과 끝에서 필요한 작업을 수행하는 클래스입니다.
 *
 * 이 프로젝트에서는 이벤트가 끝날 때마다 HitsCollection에 저장된 데이터를 읽어와,
 * 이벤트 요약 정보(EventSummary TTree)와 상세 Hit 정보(Hits TTree)를
 * G4AnalysisManager를 통해 ROOT 파일에 기록하는 핵심적인 역할을 합니다.
 */
class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  // 이벤트가 끝난 직후에 호출되어 데이터 저장 로직을 수행합니다.
  virtual void EndOfEventAction(const G4Event*) override;
};

#endif
