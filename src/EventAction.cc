#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "LSHit.hh"
#include "G4SystemOfUnits.hh"

// std::set을 사용하여 중복된 트랙을 효율적으로 제거하기 위해 헤더를 포함합니다.
#include <set>

/**
 * @brief 생성자
 */
EventAction::EventAction() : G4UserEventAction() {}

/**
 * @brief 소멸자
 */
EventAction::~EventAction() {}

/**
 * @brief 각 이벤트가 끝날 때마다 호출되는 함수입니다.
 * @param event 현재 이벤트에 대한 정보를 담고 있는 G4Event 객체 포인터
 *
 * 이 함수는 LSSD에서 수집된 HitsCollection을 분석하여,
 * 1) 이벤트 요약 정보(입자 수)를 계산하여 'EventSummary' TTree에 저장하고,
 * 2) 각 Hit의 상세 정보를 'Hits' TTree에 저장하는 두 가지 핵심 역할을 수행합니다.
 */
void EventAction::EndOfEventAction(const G4Event* event)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4int eventID = event->GetEventID();

  // "LSHitsCollection"이라는 이름으로 등록된 Hits Collection의 ID를 가져옵니다.
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("LSHitsCollection");
  if (hcID < 0) return; // 해당 Collection이 없으면 함수 종료

  // ID를 이용해 실제 Hits Collection 객체를 가져옵니다.
  auto hitsCollection = static_cast<LSHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  // Collection이 비어있으면 (즉, 이번 이벤트에서 LS 내에 hit이 없었으면) 함수 종료
  if (!hitsCollection || hitsCollection->entries() == 0) return;


  // --- 1. 이벤트 요약 정보 계산 및 저장 ('EventSummary' TTree) ---
  G4int primaryCount = 0;
  G4int secondaryCount = 0;
  // 한 이벤트 내에서 동일한 입자가 여러 번 hit을 남겨도 한 번만 세기 위해,
  // 이미 계산한 트랙의 ID를 저장하는 set을 사용합니다.
  std::set<G4int> countedTracks;

  // 이번 이벤트의 모든 Hit을 분석하여 LS에 들어온 입자 수를 셉니다.
  for (size_t i = 0; i < hitsCollection->entries(); ++i)
  {
    auto hit = (*hitsCollection)[i];
    G4int trackID = hit->GetTrackID();

    // 해당 트랙을 처음 발견한 경우에만 계수합니다.
    if (countedTracks.find(trackID) == countedTracks.end())
    {
      countedTracks.insert(trackID); // 이 트랙은 이제 계산되었다고 표시

      // ParentID가 0이면 최초 입자(primary), 아니면 2차 입자(secondary)로 간주합니다.
      if (hit->GetParentID() == 0) {
        primaryCount++;
      } else {
        secondaryCount++;
      }
    }
  }

  // 계산된 요약 정보를 EventSummary TTree (Ntuple ID=1)에 저장합니다.
  // FillNtupleIColumn(NtupleID, ColumnID, value)
  analysisManager->FillNtupleIColumn(1, 0, eventID);
  analysisManager->FillNtupleIColumn(1, 1, primaryCount);
  analysisManager->FillNtupleIColumn(1, 2, secondaryCount);
  analysisManager->AddNtupleRow(1);


  // --- 2. 개별 Hit 상세 정보 저장 ('Hits' TTree) ---
  for (size_t i = 0; i < hitsCollection->entries(); ++i)
  {
    auto hit = (*hitsCollection)[i];
    
    // Hits TTree (Ntuple ID=0)에 상세 정보를 저장합니다.
    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleIColumn(0, 1, hit->GetTrackID());
    analysisManager->FillNtupleIColumn(0, 2, hit->GetParentID());
    analysisManager->FillNtupleSColumn(0, 3, hit->GetParticleName());
    analysisManager->FillNtupleSColumn(0, 4, hit->GetProcessName());
    analysisManager->FillNtupleDColumn(0, 5, hit->GetPosition().x() / mm);
    analysisManager->FillNtupleDColumn(0, 6, hit->GetPosition().y() / mm);
    analysisManager->FillNtupleDColumn(0, 7, hit->GetPosition().z() / mm);
    analysisManager->FillNtupleDColumn(0, 8, hit->GetTime());
    analysisManager->FillNtupleDColumn(0, 9, hit->GetKineticEnergy());
    analysisManager->FillNtupleDColumn(0, 10, hit->GetEnergyDeposit());
    analysisManager->AddNtupleRow(0);
  }
}
