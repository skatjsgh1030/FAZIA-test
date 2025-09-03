#ifndef KSCINModelInelastic_h
#define KSCINModelInelastic_h 1


#include "globals.hh"
#include "G4HadronicInteraction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"

#include "KSCINFunction.hh"

class KSCINModelInelastic : public G4HadronicInteraction
{
  public: 

    KSCINModelInelastic(G4ParticleDefinition* projectile = G4Neutron::Neutron(), G4bool PSDOption=false, const char* name = "KSCINInelastic" );
    ~KSCINModelInelastic();

    G4HadFinalState * ApplyYourself(const G4HadProjectile & aTrack, G4Nucleus & aTargetNucleus);
    virtual const std::pair<G4double, G4double> GetFatalEnergyCheckLevels() const;

  virtual G4bool IsApplicable(const G4HadProjectile &, G4Nucleus &);

  public:
    void BuildPhysicsTable(const G4ParticleDefinition&);
    virtual void ModelDescription(std::ostream& outFile) const;

    void SetPSDSwitch( G4bool option ) { fSwitchPSD = option; }

  private:
    G4HadFinalState* theResult;

    G4ThreeVector GenMomDir(G4ThreeVector MomDirIn, G4double theta, G4double phi);
    G4String GetChannel(G4double Energy);

    G4double GetNCElasticXS(G4double Energy);
    G4double GetNGAMMAXS(G4double Energy);
    G4double GetN2NXS(G4double Energy);
    G4double GetN3HEXS(G4double Energy);
    G4double GetNALPHAXS(G4double Energy);
    G4double GetNDXS(G4double Energy);
    G4double GetN3AXS(G4double Energy);
    G4double GetNPXS(G4double Energy);
    G4double GetNPNXS(G4double Energy);
    G4double GetNTXS(G4double Energy);

    G4bool fSwitchPSD = false;

    G4int Neutron = 0;
    G4int Proton = 1;
    G4int Alpha = 2;
    G4int Be9 = 3;
    G4int B11 = 4;
    G4int B12 = 5;
    G4int C12 = 6;
    G4int Be8 = 7;
    G4int C11 = 8;
    G4int Deuteron = 9;
    G4int He5 = 10;
    G4int B10 = 11;
    G4int Triton = 12;
    G4int Be11 = 13;
    G4int Be10 = 14;
    G4int Li8 = 15;
    //G4int Li7 = 16;
    //G4int Li6 = 17;
    G4int He3 = 18;

    G4int fEventBef = 0;
    G4double fEneut2 = 0.0;
};

#endif
