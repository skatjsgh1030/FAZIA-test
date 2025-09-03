#ifndef NxQMDCollision_hh
#define NxQMDCollision_hh

#include "NxQMDSystem.hh"
#include "NxQMDMeanField.hh"

#include "G4Scatterer.hh"

class NxQMDCollision 
{
   public:
      NxQMDCollision();
      ~NxQMDCollision();

      void CalKinematicsOfBinaryCollisions( G4double );
      void PionAbsorption( G4double );
      void PionEmission( G4int id, G4double dt );

      //G4bool CalFinalStateOfTheBinaryCollision( G4int , G4int );
      G4bool CalFinalStateOfTheBinaryCollisionJQMD( G4double , G4double , G4ThreeVector , G4double , G4double , G4ThreeVector , G4double , G4int , G4int );
      //     CalFinalStateOfTheBinaryCollision ( sig , cutoff , pcm , prcm , srt, beta , gamma , i , j );

      void SetMeanField ( NxQMDMeanField* meanfield ){ theMeanField = meanfield; theSystem = meanfield->GetSystem(); }
      void SetNucNucColl ( G4bool collType ){ NucNucColl = collType; }

      // Get, Set method of parameters; For expert only 
      void deltar( G4double x ){ fdeltar = x; };
      G4double deltar(){ return fdeltar; };
      void bcmax0( G4double x ){ fbcmax0 = x; };
      G4double bcmax0(){ return fbcmax0; };
      void bcmax1( G4double x ){ fbcmax1 = x; };
      G4double bcmax1(){ return fbcmax1; };
      void epse( G4double x ){ fepse = x; };
      G4double epse(){ return fepse; };


   private:
      //copy is unexpeced
      NxQMDCollision( const NxQMDCollision& ){;};
      const NxQMDCollision& operator= ( const NxQMDCollision& );

      NxQMDSystem* theSystem;
      NxQMDMeanField* theMeanField;
      G4double GetSOO(G4int, G4double);
      void RESMAS(G4int, G4int, G4int, G4double, G4double&, G4double&, G4double&, G4double&);
      G4double PIDENO(G4int, G4double, G4double, G4double);

      G4double deltaT;

      G4double fdeltar;
      G4double fbcmax0 , fbcmax1;
      G4double sig0 , sig1;
      G4double fepse; 
      G4bool NucNucColl = false;

      G4double rmass = 0.9383;
      G4double pmass = 0.138;
      G4double smass = 1.440;
      G4double dmass = 1.232;

      G4Scatterer* theScatterer;
};

#endif
