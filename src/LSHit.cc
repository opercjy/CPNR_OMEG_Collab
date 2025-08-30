#include "LSHit.hh"

// LSHit 객체를 위한 메모리 할당자를 G4ThreadLocal로 선언 및 초기화합니다.
// 멀티스레드 환경에서 각 스레드가 자신만의 메모리 풀을 갖도록 하여 성능을 향상시킵니다.
G4ThreadLocal G4Allocator<LSHit>* LSHitAllocator = nullptr;

/**
 * @brief 생성자: 모든 멤버 변수를 기본값으로 초기화합니다.
 */
LSHit::LSHit()
: G4VHit(),
  fTrackID(0), fParentID(0),
  fParticleName(""), fProcessName(""),
  fPosition(0,0,0), fTime(0.),
  fKineticEnergy(0.), fEnergyDeposit(0.)
{}

/**
 * @brief 소멸자
 */
LSHit::~LSHit()
{}
