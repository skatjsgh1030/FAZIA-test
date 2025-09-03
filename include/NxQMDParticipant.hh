#ifndef NxQMDParticipant_hh
#define NxQMDParticipant_hh

#include <CLHEP/Units/SystemOfUnits.h>

#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"

class NxQMDParticipant 
{
   public:
                                                // momentum      position
      NxQMDParticipant( const G4ParticleDefinition* , G4ThreeVector , G4ThreeVector );
      ~NxQMDParticipant();

      void SetDefinition( const G4ParticleDefinition* pd ) { definition = pd; };
      const G4ParticleDefinition* GetDefinition() { return definition; };

      void SetPosition( G4ThreeVector r ) { position = r; };
      G4ThreeVector GetPosition() { return position; };

      void SetMomentum( G4ThreeVector p ) { momentum = p; };
      G4ThreeVector GetMomentum() { return momentum; };

      G4double GetMass() { return definition->GetPDGMass()/CLHEP::GeV; };

      G4LorentzVector Get4Momentum();

      //G4double GetKineticEnergy() { return Get4Momentum().e() - GetMass(); };
      G4double GetKineticEnergy() { return Get4Momentum().e() - fDem; };

      G4int GetBaryonNumber() { return definition->GetBaryonNumber(); };
      G4int GetNuc() { return iNuc; };
      void SetNuc( G4int nuc ) { iNuc = nuc; };

      G4int GetiNun() { return iNun; }
      void SetiNun( G4int i ) { iNun = i; }

      G4int GetChargeInUnitOfEplus() { return int ( definition->GetPDGCharge()/CLHEP::eplus ); };

      void UnsetInitialMark() { projectile = false; target = false; }
      void UnsetHitMark() { hit = false; }
      G4bool IsThisHit() { return hit; }
      void SetHitMark() { hit = true; }

      void SetProjectile() { projectile = true; }
      void SetTarget() { target = true; }
      G4bool IsThisProjectile() { return projectile; }
      G4bool IsThisTarget() { return target; }

      void SetDem( G4double dem ) { fDem = dem; }
      G4double GetDem() { return fDem; }

   private:
      const G4ParticleDefinition* definition;
      G4ThreeVector momentum;
      G4ThreeVector position;

      G4bool projectile; 
      G4bool target; 
      G4bool hit; 

      G4double fDem = 0;
      G4int iNuc = 0;
      G4int iNun = -1;
};

#endif
