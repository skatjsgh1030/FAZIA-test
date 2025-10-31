#ifndef SETUPDETECTOR_HH
#define SETUPDETECTOR_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4NistManager;

class G4PhysicalVolume;
class G4LogicalVolume;

class SetUpDetector
{
    public:

            SetUpDetector();
      virtual ~SetUpDetector();

      void SetUpDetectorGeo(G4LogicalVolume* logicWorld);
      void SetRotationDeg(G4double rotDeg);
		
		private:
			G4NistManager* nist = nullptr;
    	
			G4double fRotDeg = 0;

};

#endif

