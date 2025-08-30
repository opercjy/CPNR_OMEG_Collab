#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

/**
 * @class PrimaryGeneratorAction
 * @brief 각 이벤트의 초기 입자를 생성하는 역할을 담당하는 클래스입니다.
 *
 * G4GeneralParticleSource(GPS)를 사용하여, 매크로 파일(.mac)에서
 * 소스의 종류, 위치, 에너지, 방출 각도 등 다양한 설정을 유연하게 제어할 수 있도록 합니다.
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  // Geant4 커널이 매 이벤트 시작 시 호출하는 함수입니다.
  virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
  G4GeneralParticleSource* fGPS;
};

#endif
