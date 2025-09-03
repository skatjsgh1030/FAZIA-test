#ifndef NxQMDGroundStateNucleus_hh
#define NxQMDGroundStateNucleus_hh

#include "NxQMDNucleus.hh"
#include "NxQMDMeanField.hh"

#include "NxQMDParameters.hh"

class NxQMDGroundStateNucleus : public NxQMDNucleus
{
   public:
      NxQMDGroundStateNucleus();
      NxQMDGroundStateNucleus( G4int z , G4int a );
      ~NxQMDGroundStateNucleus()
      {
         rho_l.clear();  
         d_pot.clear();
      };
                             


   private:

      void packNucleons();

      void killCMMotionAndAngularM();

      G4int maxTrial;
      G4bool samplingPosition( G4int );

      G4bool samplingMomentum( G4int );

      G4double r00 , r01 , saa , rada , radb;

      G4double dsam, ddif, dsam2, ddif2; 

      G4double cdp;
      G4double c0p,clp,c3p,csp;

      G4double hbc;
      G4double gamm;
      G4double cpw;
      G4double cph;
      G4double epsx;
      G4double cpc;

      G4double rmax,rt00,radm; 

      std::vector< G4double > phase_g;

      std::vector< G4double > rho_l;
      std::vector< G4double > d_pot;
      G4double ebini;
      G4double edepth;
      G4double epse;

      
      NxQMDMeanField* meanfield;
};

#endif
