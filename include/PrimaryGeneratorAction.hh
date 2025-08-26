#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

//______________________________________________________________________________________
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  // 각 이벤트가 시작될 때마다 호출되어 초기 입자를 생성합니다.
  virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
  G4GeneralParticleSource* fGPS; // GPS 객체 포인터
};

#endif
