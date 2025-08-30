#include "LSSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

/**
 * @brief 생성자
 * @param name Sensitive Detector의 고유한 이름
 */
LSSD::LSSD(const G4String& name)
: G4VSensitiveDetector(name), fHitsCollection(nullptr)
{
  // 이 SD가 생성할 Hits Collection의 이름을 등록합니다.
  collectionName.insert("LSHitsCollection");
}

/**
 * @brief 소멸자
 */
LSSD::~LSSD()
{}

/**
 * @brief 각 이벤트가 시작될 때 Geant4 커널에 의해 호출됩니다.
 * @param hce 현재 이벤트의 모든 Hits Collection들을 담는 컨테이너
 *
 * 이 함수는 이번 이벤트에서 생성될 Hit들을 담을 새로운 Hits Collection 객체를 만들고,
 * 이벤트 컨테이너(hce)에 등록하는 역할을 합니다.
 */
void LSSD::Initialize(G4HCofThisEvent* hce)
{
  // LSHitsCollection 객체를 생성합니다.
  fHitsCollection = new LSHitsCollection(SensitiveDetectorName, collectionName[0]);
  
  // 이 Collection을 Geant4 시스템에 등록하고, 고유 ID를 받아옵니다.
  G4int hcID = GetCollectionID(0);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

/**
 * @brief 이 SD가 부착된 볼륨 내에서 입자가 한 스텝을 진행할 때마다 호출됩니다.
 * @param aStep 현재 스텝에 대한 모든 정보를 담고 있는 G4Step 객체 포인터
 * @return bool Hit이 성공적으로 처리되었는지 여부
 */
G4bool LSSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{
  // 해당 스텝에서 손실된 에너지가 0이면, 굳이 기록할 필요가 없으므로 함수를 종료합니다.
  if (aStep->GetTotalEnergyDeposit() == 0.) return false;

  // 새로운 Hit 객체를 생성합니다.
  LSHit* newHit = new LSHit();
  
  // 현재 스텝의 트랙 정보를 가져옵니다.
  G4Track* track = aStep->GetTrack();
  
  // Hit 객체에 각종 물리량을 채워 넣습니다.
  newHit->SetTrackID(track->GetTrackID());
  newHit->SetParentID(track->GetParentID());
  newHit->SetParticleName(track->GetDefinition()->GetParticleName());
  
  // 추가: 현재 스텝의 논리 볼륨 이름을 가져와 Hit에 저장
  G4String volumeName = track->GetVolume()->GetLogicalVolume()->GetName();
  newHit->SetVolumeName(volumeName);

  // 입자를 생성시킨 물리 프로세스의 이름을 기록합니다.
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if (creatorProcess) {
    newHit->SetProcessName(creatorProcess->GetProcessName());
  } else {
    // 생성 프로세스가 없는 경우 (최초 입자) "primary"로 기록합니다.
    newHit->SetProcessName("primary");
  }

  // PreStepPoint: 스텝이 시작되는 지점의 정보를 가져옵니다.
  newHit->SetPosition(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime() / ns);
  newHit->SetKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy() / MeV);
  
  // GetTotalEnergyDeposit(): 해당 스텝 동안 물질에 흡수된 총 에너지를 가져옵니다.
  newHit->SetEnergyDeposit(aStep->GetTotalEnergyDeposit() / MeV);

  // 완성된 Hit 객체를 이번 이벤트의 Hits Collection에 추가합니다.
  fHitsCollection->insert(newHit);

  return true;
}
