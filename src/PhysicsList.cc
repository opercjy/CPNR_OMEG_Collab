#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"

//______________________________________________________________________________________
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // 1. Decay Physics
  RegisterPhysics(new G4DecayPhysics());

  // 2. Radioactive Decay Physics (핵심)
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // 3. Electromagnetic Physics (가장 정밀한 표준 모델)
  RegisterPhysics(new G4EmStandardPhysics_option4());

  // 4. Hadronic Physics (FTFP_BERT 기반)
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4IonPhysics());
  RegisterPhysics(new G4StoppingPhysics());
}

//______________________________________________________________________________________
PhysicsList::~PhysicsList()
{
}
