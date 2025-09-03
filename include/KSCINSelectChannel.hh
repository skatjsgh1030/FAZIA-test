#ifndef KSCINSELECTCHANNEL_HH
#define KSCINSELECTCHANNEL_HH

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class KSCINSelectChannel
{
  public:
    KSCINSelectChannel();
    ~KSCINSelectChannel(){};

    G4int Get3AlphaChannel(G4double NeutronEnergy);
    G4int Get12BChannel(G4double NeutronEnergy);

    G4double Get8LiExFrom12B(G4double ExcitationEnergy);
    G4double Get8LiExFrom9Be(G4double ExcitationEnergy);
    G4double GetProbProtDecay(G4double ExcitationEnergy);
    G4double GetProbPP2P(G4double ExcitationEnergy);
    G4double GetProbPpalpha(G4double ExcitationEnergy);

};
#endif
