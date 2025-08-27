#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

// G4GeneralParticleSource 클래스를 사용하기 위한 전방 선언
class G4GeneralParticleSource;
class G4Event;

//______________________________________________________________________________________
// 각 이벤트의 초기 입자를 생성하는 역할을 담당하는 클래스입니다.
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  // Geant4 커널이 매 이벤트 시작 시 호출하는 함수입니다.
  // 이 함수 안에서 초기 입자를 생성하고 이벤트에 추가합니다.
  virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
  // General Particle Source (GPS) 객체 포인터.
  // GPS를 사용하면 매크로 파일에서 다양한 소스 설정을 쉽게 할 수 있습니다.
  G4GeneralParticleSource* fGPS;
};

#endif
