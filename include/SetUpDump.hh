#ifndef SETUPDUMP_HH
#define SETUPDUMP_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

class G4PhysicalVolume;
class G4LogicalVolume;

class SetUpDump
{
    public:

      SetUpDump();
      virtual ~SetUpDump();

      void SetUpDumpGeo(G4LogicalVolume* logicWorld);
};

#endif

