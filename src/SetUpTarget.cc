#include "SetUpTarget.hh"
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
using namespace std;

SetUpTarget::SetUpTarget(){}
SetUpTarget::~SetUpTarget(){}

void SetUpTarget::SetUpTargetGeo(G4LogicalVolume* logicWorld)
{
  // ========================= Target ========================
  auto* nist = G4NistManager::Instance();
  G4Material* Au = nist->FindOrBuildMaterial("G4_Au");
  G4double target_size = 0.02*m;
  G4double target_Thickness = 6.*um;

	G4VisAttributes* SetUpTargetVisAtt;
  
	auto* solidTarget = new G4Box("solidTarget", target_size/2., target_size/2., target_Thickness/2.);
  auto* logicTarget = new G4LogicalVolume(solidTarget, Au, "logicTarget");
  (void) new G4PVPlacement( nullptr, G4ThreeVector(0.,0.,0.*cm), logicTarget, "physTarget", logicWorld, false, 1, true); // Copy number of 1 for Target

  auto* detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
  detVisAtt->SetForceSolid(false);
  
	logicTarget->SetVisAttributes(detVisAtt);

	SetUpTargetVisAtt = new G4VisAttributes(G4Colour(0.1,0.4,0.7));
	SetUpTargetVisAtt -> SetForceWireframe(true);
  logicTarget -> SetVisAttributes (SetUpTargetVisAtt);
}
