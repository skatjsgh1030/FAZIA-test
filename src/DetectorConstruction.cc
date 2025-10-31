// DetectorConstruction.cc
#include "DetectorConstruction.hh"

#include "SetUpDetector.hh"
#include "SetUpCollimator.hh"
#include "SetUpTarget.hh"

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

DetectorConstruction::DetectorConstruction( G4double rotDeg )
	: G4VUserDetectorConstruction()
{
	fRotDeg = rotDeg;
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	nist = G4NistManager::Instance();

	// ========================= World =========================
	//G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
	auto* solidWorld = new G4Box("solidWorld", 3*m, 3*m, 3*m);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	auto* physWorld  = new G4PVPlacement( nullptr, G4ThreeVector(0.,0.,0.), logicWorld, "phyWorld", nullptr, false, 0, true); // Copy number of 0 for World

	logicWorld -> SetVisAttributes(G4VisAttributes::GetInvisible());

	SetUpCollimator coll_1;
	SetUpTarget targ;
	SetUpDump dump;
	SetUpDetector dete;
	// 회전각 전달
	dete.SetRotationDeg(fRotDeg);

    fprintf(stderr, "[DetectorConstruction] Call SetUpCollimator\n");
	if(0){coll_1.SetUpCollimatorGeo(logicWorld);} 
    fprintf(stderr, "[DetectorConstruction] Call SetUpTarget\n");
	if(1){targ.SetUpTargetGeo(logicWorld);} 
    fprintf(stderr, "[DetectorConstruction] Call SetUpDump\n");
	if(1){dump.SetUpDumpGeo(logicWorld);} 
    fprintf(stderr, "[DetectorConstruction] Call SetUpDetector\n");
	if(1){dete.SetUpDetectorGeo(logicWorld);} 

	return physWorld;
}

