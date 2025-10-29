#ifndef SETUPTARGET_HH
#define SETUPTARGET_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

class G4PhysicalVolume;
class G4LogicalVolume;

class SetUpTarget
{
    public:

      SetUpTarget();
      virtual ~SetUpTarget();

      void SetUpTargetGeo(G4LogicalVolume* logicWorld);
};

#endif

