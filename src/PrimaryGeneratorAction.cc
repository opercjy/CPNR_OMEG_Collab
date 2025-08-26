#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

//______________________________________________________________________________________
PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fGPS(0)
{
  fGPS = new G4GeneralParticleSource();
}

//______________________________________________________________________________________
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGPS;
}

//______________________________________________________________________________________
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fGPS->GeneratePrimaryVertex(anEvent);
}
