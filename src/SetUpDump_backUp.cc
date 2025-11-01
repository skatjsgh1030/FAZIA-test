#include "SetUpDump.hh"
#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"

#include "G4RunManager.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Sphere.hh"
//#include "G4Tubs.hh"

#include "G4Element.hh"
#include "G4Material.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4RotationMatrix.hh"
#include "G4AssemblyVolume.hh"

#include <cmath> // std::sin, std::cos

using namespace CLHEP;
using namespace std;

SetUpDump::SetUpDump(){}
SetUpDump::~SetUpDump(){}

void SetUpDump::SetUpDumpGeo(G4LogicalVolume* logicWorld)
{
  // ========================= Dump ========================
  auto* nist = G4NistManager::Instance();
  G4Material* Au = nist->FindOrBuildMaterial("G4_Pb");
  G4double outer_radius = 0.06*m;
  G4double inner_radius = 0.04*m;

	G4VisAttributes* SetUpDumpVisAtt;
  
	auto* solidDump = new G4Sphere("solidDump", inner_radius, outer_radius, 0.*deg, 360.*deg, 0.*deg, 90.*deg);
  auto* logicDump = new G4LogicalVolume(solidDump, Au, "logicDump");
  (void) new G4PVPlacement( nullptr, G4ThreeVector(0.,0.,0.*cm), logicDump, "physDump", logicWorld, false, 99, true); // Copy number of 1 for Dump

  auto* detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
  detVisAtt->SetForceSolid(true);
  
	logicDump->SetVisAttributes(detVisAtt);

	SetUpDumpVisAtt = new G4VisAttributes(G4Colour(0.5,0.4,0.7));
	SetUpDumpVisAtt -> SetForceWireframe(true);
  logicDump -> SetVisAttributes (SetUpDumpVisAtt);
}
