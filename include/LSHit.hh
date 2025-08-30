#ifndef LSHit_h
#define LSHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"

/**
 * @class LSHit
 * @brief Sensitive Detector에서 발생하는 각 상호작용(Hit)의 정보를 저장하는 데이터 클래스입니다.
 *
 * Geant4의 G4VHit을 상속받으며, 한 스텝(step)에서 얻고자 하는 모든 물리량을 멤버 변수로 가집니다.
 * 이 클래스의 객체들은 LSHitsCollection에 담겨 EventAction으로 전달됩니다.
 * 메모리 효율성을 위해 G4Allocator를 사용한 custom new/delete 연산자를 오버로딩합니다.
 */
class LSHit : public G4VHit
{
public:
  LSHit();
  virtual ~LSHit();

  // Custom new/delete for memory management
  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // --- Setters and Getters ---
  void SetTrackID(G4int id) { fTrackID = id; }
  G4int GetTrackID() const { return fTrackID; }

  void SetParentID(G4int id) { fParentID = id; }
  G4int GetParentID() const { return fParentID; }

  void SetParticleName(const G4String& name) { fParticleName = name; }
  const G4String& GetParticleName() const { return fParticleName; }
  
  void SetProcessName(const G4String& name) { fProcessName = name; }
  const G4String& GetProcessName() const { return fProcessName; }

  void SetPosition(const G4ThreeVector& pos) { fPosition = pos; }
  const G4ThreeVector& GetPosition() const { return fPosition; }
  
  void SetTime(G4double t) { fTime = t; }
  G4double GetTime() const { return fTime; }

  void SetKineticEnergy(G4double e) { fKineticEnergy = e; }
  G4double GetKineticEnergy() const { return fKineticEnergy; }

  void SetEnergyDeposit(G4double edep) { fEnergyDeposit = edep; }
  G4double GetEnergyDeposit() const { return fEnergyDeposit; }

private:
  G4int         fTrackID;        // 현재 입자의 트랙 ID
  G4int         fParentID;       // 부모 입자의 트랙 ID
  G4String      fParticleName;   // 입자 이름
  G4String      fProcessName;    // 입자를 생성시킨 물리 프로세스 이름
  G4ThreeVector fPosition;       // Hit 발생 위치 (x, y, z)
  G4double      fTime;           // Hit 발생 시간
  G4double      fKineticEnergy;  // Hit 발생 시점의 운동 에너지
  G4double      fEnergyDeposit;  // 해당 스텝에서 잃은 에너지
};

// LSHit 객체들을 담을 컨테이너 타입 정의
typedef G4THitsCollection<LSHit> LSHitsCollection;

// 메모리 할당자(Allocator)를 전역 변수로 선언 (G4ThreadLocal)
extern G4ThreadLocal G4Allocator<LSHit>* LSHitAllocator;

inline void* LSHit::operator new(size_t)
{
  if (!LSHitAllocator) LSHitAllocator = new G4Allocator<LSHit>;
  return (void*)LSHitAllocator->MallocSingle();
}

inline void LSHit::operator delete(void* aHit)
{
  LSHitAllocator->FreeSingle((LSHit*)aHit);
}

#endif
