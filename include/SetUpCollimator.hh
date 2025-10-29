#ifndef SETUPCOLLIMATOR_HH
#define SETUPCOLLIMATOR_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

class G4PhysicalVolume;
class G4LogicalVolume;

class SetUpCollimator
{
    public:

      SetUpCollimator();
      virtual ~SetUpCollimator();

      void SetUpCollimatorGeo(G4LogicalVolume* logicWorld);
		
		private:			
			G4NistManager* nist = nullptr;
};

#endif

