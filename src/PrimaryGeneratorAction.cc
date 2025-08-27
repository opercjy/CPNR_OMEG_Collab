// PrimaryGeneratorAction.cc
// PrimaryGeneratorAction 클래스의 멤버 함수들을 구현합니다.

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh" // GPS 헤더 파일 포함

//______________________________________________________________________________________
/**
 * @brief 생성자 (Constructor)
 *
 * PrimaryGeneratorAction 객체가 생성될 때 호출됩니다.
 * G4GeneralParticleSource(GPS) 객체를 생성하여 멤버 변수에 할당합니다.
 */
PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fGPS(nullptr)
{
  fGPS = new G4GeneralParticleSource();
}

//______________________________________________________________________________________
/**
 * @brief 소멸자 (Destructor)
 *
 * PrimaryGeneratorAction 객체가 소멸될 때 호출됩니다.
 * 생성했던 GPS 객체를 삭제하여 메모리 누수를 방지합니다.
 */
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGPS;
}

//______________________________________________________________________________________
/**
 * @brief GeneratePrimaries
 * @param anEvent 현재 이벤트에 대한 정보를 담고 있는 객체 포인터
 *
 * 매 이벤트 시작 시 Geant4 커널에 의해 호출됩니다.
 * GPS가 매크로 파일(.mac)에 정의된 설정에 따라 입자를 생성하여
 * 현재 이벤트(anEvent)에 주입하는 역할을 합니다.
 */
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fGPS->GeneratePrimaryVertex(anEvent);
}
