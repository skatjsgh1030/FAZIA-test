#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction(G4double Energy)
  : G4VUserPrimaryGeneratorAction()
    , fEnergy(Energy)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable -> FindParticle(particleName = "proton");

  fParticleGun = new G4ParticleGun();
  fParticleGun -> SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) 
{
  G4ThreeVector gunPosition = G4ThreeVector(0, 0, 0);
  fParticleGun -> SetParticlePosition(gunPosition);

  G4double x, y, z;

  x = 0.;
  y = 0.;
  z = 1.;

  G4double KinEnergy = fEnergy*MeV;

  fParticleGun -> SetParticleMomentumDirection( G4ThreeVector(x,y,z) );
  fParticleGun -> SetParticleEnergy( KinEnergy );

  fParticleGun -> GeneratePrimaryVertex(anEvent);
}
