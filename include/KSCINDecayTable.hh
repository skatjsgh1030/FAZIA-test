#ifndef KSCINDECAYTABLE_HH
#define KSCINDECAYTABLE_HH

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "KSCINFunction.hh"
#include "KSCINSelectChannel.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4VParticleChange.hh"

class KSCINDecayTable
{
  public:
  KSCINDecayTable();
  ~KSCINDecayTable();

  void Li6todpa(G4double Excitation, G4double V6Li, std::vector<G4DynamicParticle*> &totalDP);
  void Li7totpa(G4double Excitation, G4double V7Li, std::vector<G4DynamicParticle*> &totalDP);
  void Li8Decay(G4double Excitation, G4double V8Li, G4double &Eg, std::vector<G4DynamicParticle*> &totalDP, G4double &Eneut2);
  void Be9toLi7pd(G4double Excitation, G4double V9Be, std::vector<G4DynamicParticle*> &totalDP);
  void Be9toLi8pp(G4double Excitation, G4double V9Be, G4double &Eg, std::vector<G4DynamicParticle*> &totalDP, G4double &Eneut2 );
  void B10Decay(G4double Excitation, G4double V10B, G4double &Eg, std::vector<G4DynamicParticle*> &totalDP,  G4int IsNeutronEmitted, std::vector<G4DynamicParticle*> &removeDP);
  void B11to7Lipa(G4double Excitation, G4double V11BLab, G4double &Eg, G4bool FromND, std::vector<G4DynamicParticle*> &totalDP, std::vector<G4DynamicParticle*> &removeDP, G4double &Eneut2);
  void B11toB10pn(G4double Excitation, G4double EnInit, G4double Q_value, G4double V11BLab, G4double &Eg, G4double &Eleft, std::vector<G4DynamicParticle*> &totalDP, std::vector<G4DynamicParticle*> &removeDP, G4double &Eneut2);

  G4double GetVx1() { return Vx1; }
  G4double GetVy1() { return Vy1; }
  G4double GetVz1() { return Vz1; }
  G4double GetVx2() { return Vx2; }
  G4double GetVy2() { return Vy2; }
  G4double GetVz2() { return Vz2; }

  G4double GetE1() { return E1; }
  G4double GetE2() { return E2; }

  private:
  KSCINFunction SFunction;
  KSCINSelectChannel Select;

  G4double Vx1 = 0;
  G4double Vy1 = 0;
  G4double Vz1 = 0;
  G4double Vx2 = 0;
  G4double Vy2 = 0;
  G4double Vz2 = 0;
  G4double E1 = 0;
  G4double E2 = 0;

    G4int Neutron = 0;
    G4int Proton = 1;
    G4int Alpha = 2;
    G4int Be9 = 3;
    //G4int B11 = 4; // No use
    //G4int B12 = 5; // No use
    //G4int C12 = 6; // No use
    G4int Be8 = 7;
    //G4int C11 = 8; // No use
    G4int Deuteron = 9;
    //G4int He5 = 10; // No use
    G4int B10 = 11;
    G4int Triton = 12;
    //G4int Be11 = 13; // No use
    //G4int Be10 = 14; // No use
    G4int Li8 = 15;
    G4int Li7 = 16;
    G4int Li6 = 17;
    //G4int He3 = 18; // No use
};

#endif
