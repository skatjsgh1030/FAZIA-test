#include "SetUpCollimator.hh"
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
#include "G4Box.hh"
#include "G4Tubs.hh"

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

SetUpCollimator::SetUpCollimator(){}
SetUpCollimator::~SetUpCollimator(){}

void SetUpCollimator::SetUpCollimatorGeo(G4LogicalVolume* logicWorld)
{
    nist = G4NistManager::Instance();
    fprintf(stderr, "[SetUpCollimator] Start, nist=%p\n", (void*)nist);
	// ========================= Collimator ========================
  G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");
  G4double inner_r = 0.15* cm;
  G4double outer_r = 3.* cm;
  G4double collimator_thickness = 4.* cm;

  auto* solidCollimator = new G4Tubs("solidCollimator", inner_r, outer_r, collimator_thickness/2.,0.*deg, 360.*deg);
  auto* logicCollimator = new G4LogicalVolume(solidCollimator, Pb, "logicCollimator");
  (void) new G4PVPlacement( nullptr, G4ThreeVector(0.,0.,-3.*cm), logicCollimator, "physCollimator", logicWorld, false, 2, true); // Copy number of 2 for Collimator

	G4VisAttributes* SetUpCollimatorVisAtt;

	logicWorld -> SetVisAttributes(G4VisAttributes::GetInvisible());
	
	SetUpCollimatorVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
 	SetUpCollimatorVisAtt -> SetForceWireframe(true);
 	logicCollimator -> SetVisAttributes (SetUpCollimatorVisAtt);
}
