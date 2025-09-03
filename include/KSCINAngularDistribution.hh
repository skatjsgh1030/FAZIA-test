#ifndef KSCINANGULARDISTRIBUTIONL_HH
#define KSCINANGULARDISTRIBUTIONL_HH

#include "globals.hh"

class KSCINAngularDistribution
{
  public:
    KSCINAngularDistribution(){};
    ~KSCINAngularDistribution(){};
	
	void GetNAlphaArray(G4double Energy, G4double* Array);
	void GetNTArray(G4double Energy, G4double* Array);
	void GetN3HeArray(G4double Energy, G4double* Array);
};

#endif
