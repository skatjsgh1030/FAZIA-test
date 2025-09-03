#ifndef KSCINFUNCTION_HH
#define KSCINFUNCTION_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
//#include "TF1.h"

using namespace CLHEP;

class KSCINFunction
{
  public:
    KSCINFunction();
    ~KSCINFunction();

    G4double EXTERP(G4double *a, G4double *b, G4double c, G4int N, G4int Ntype); // INTERPOLATION FOR ANGULARDISTRIBUTION
    G4double CHOOSL(G4double *Fi, G4int N); // Legendre polynomials
    G4double VELOCITY(G4int N, G4double Energy); // Calculate Velocity from Energy
    G4double RCKE(G4int N1, G4int N2, G4double TotalKineticEnergy); // Relativistic Correct Kinetic Energy
    void CMTRAN(G4int N1, G4int N2, G4double v1, G4double &vCM, G4double &Energy1, G4double &Energy2); // CM Transfrom
    void LABTRAN(G4double Vxp, G4double Vyp, G4double Vzp, G4double Vcom, G4double &Vx, G4double &Vy, G4double &Vz); // Lab frame Transfrom
    G4double EFROMV(G4int N, G4double V); // Energy from Velocity
    void RVECT(G4double &x, G4double &y, G4double &z);
    void DIRCOS(G4ThreeVector V, G4ThreeVector& cV);
    void TRANSVEC(G4ThreeVector Z, G4ThreeVector Vp, G4ThreeVector& V);

    G4double GetProtonEnergy(G4double Emax, G4double En);
    G4double GetDeuteronEnergy(G4double Emax, G4double En);
    G4double GetTritonEnergy(G4double Emax, G4double En);
    G4double Get3HeEnergy(G4double Emax, G4double En);
    G4double GetAlphaEnergy1(G4double Emax);
    G4double GetAlphaEnergy2(G4double Emax);
    G4double GetNeutronEnergy(G4double Elow, G4double Emax, G4double Parameter);

    G4double GetEmass(G4int N) {return fEmass[N];}

    G4double GetCosTheta( G4double KE, G4int Z, G4int A );
    G4double GetReactionCrossSection( G4int type, G4int iz, G4int ia, G4double Energy, G4int xstype );

  private:
    G4double fEmass[19]= // Mass of particles in terms of Energy;
    {
      939.583,938.8,3728.43,8394.86,10255.2,11191.4,11178.0,
        7456.95,10257.17,1876.14,4668.9,9327.05,2809.45,10266.69,9327.62,
        7472.96,6535.42,5603.097,2809.44
    };

    //G4double c = 2.99792458e+8 * m/s; // Speed of Light
    G4double D1 = 1.0;

    G4double dsdarc(G4double, G4int, G4double, G4double);
    G4double bessel( G4double );
    void syscrc( G4double, G4int A, G4double , G4double &, G4double &, G4double &);

};

#endif
