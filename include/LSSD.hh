#ifndef LSSD_h
#define LSSD_h 1

#include "G4VSensitiveDetector.hh"
#include "LSHit.hh"

class G4Step;
class G4HCofThisEvent;

/**
 * @class LSSD
 * @brief 액체섬광체(Liquid Scintillator)를 위한 Sensitive Detector 클래스입니다.
 *
 * 특정 논리 볼륨(LogicLS)에 부착되어, 해당 볼륨 내에서 입자가 한 스텝 진행할 때마다
 * ProcessHits() 함수가 호출됩니다. 이 함수 안에서 LSHit 객체를 생성하고 물리량을 기록합니다.
 */
class LSSD : public G4VSensitiveDetector
{
public:
  LSSD(const G4String& name);
  virtual ~LSSD();

  // 이벤트 시작 시 호출되어 Hits Collection을 준비합니다.
  virtual void Initialize(G4HCofThisEvent* hce) override;

  // 스텝이 발생할 때마다 호출되어 Hit 데이터를 처리합니다.
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

private:
  LSHitsCollection* fHitsCollection; // 생성된 LSHit들을 담을 컨테이너
};

#endif
