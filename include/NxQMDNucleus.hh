#ifndef NxQMDNucleus_hh
#define NxQMDNucleus_hh

#include "NxQMDSystem.hh"
#include "NxQMDParameters.hh"

class NxQMDNucleus : public NxQMDSystem
{
   public:
      NxQMDNucleus();
      //virtual ~NxQMDNucleus();

      G4LorentzVector Get4Momentum();

      // Number of Nucleons (Proton or Neutron)
      G4int GetMassNumber();

      // Number of Protons
      G4int GetAtomicNumber();

      void CalEnergyAndAngularMomentumInCM();

      // rest mass from G4NucleiPropertiesTable
      G4double GetNuclearMass();

      void SetTotalPotential( G4double x ){ potentialEnergy = x; };
      G4double GetExcitationEnergy(){ return excitationEnergy; };
      G4double BindingEnergy( G4int Z, G4int N );

      G4int GetAngularMomentum(){ return jj; };

   private:

      G4double hbc;

      std::vector < G4ThreeVector > rcm, pcm;
      std::vector < G4double > es;
      G4int jj;

      G4double potentialEnergy;
      G4double excitationEnergy;
      //G4double bindingEnergy;

      //G4double kineticEnergyPerNucleon;
      //G4double bindingEnergyPerNucleon;
      //G4double potentialEnergyPerNucleon;
};

#endif
