#include "DetectorConstruction.hh"

#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"

#include "G4RunManager.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4RotationMatrix.hh"

#include "G4DataInterpolation.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4UnionSolid.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{

}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
   G4NistManager *nist = G4NistManager::Instance();
    
// world 정의     
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box *solidWorld = new G4Box("solidWorld", 3*m, 3*m, 3*m );
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld,"phyWorld", 0, false, 0, true);
   
//타겟정의
    G4Material* C = nist->FindOrBuildMaterial("G4_C"); 
    G4Material* Au = nist->FindOrBuildMaterial("G4_Au"); 
    G4Box *solidTarget = new G4Box("solidTarget",0.02*m,0.02*m,10.0*um);
    G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget,C,"logicTarget");
    G4VPhysicalVolume *physTarget = new G4PVPlacement(0,G4ThreeVector(0., 0., 10.*cm),logicTarget,"physTarget",logicWorld, false, 0, true);
    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5)); 
    detVisAtt->SetForceSolid(true);
    logicTarget->SetVisAttributes(detVisAtt);

//si detector 정의
    G4double SiSize = 2.*cm;
    G4double gapThickness = 2.*mm;
    G4double alThickness = 1.*um;
    G4Material* Si = nist->FindOrBuildMaterial("G4_Si"); 
    G4Material* Al = nist->FindOrBuildMaterial("G4_Al");

    //si1 (thin)
    G4double Si1subThickness = 15.*um;
    G4double Si1epiThickness = 100.*um;
    //G4double totalSi1Thickness = alThickness + Si1subThickness + Si1epiThickness;
    G4double pos_si1 = 0.4*m; 

    G4Box *solidSi1_al = new G4Box("solidSi1_al",SiSize/2,SiSize/2,alThickness/2);
    G4LogicalVolume *logicSi1_al = new G4LogicalVolume(solidSi1_al,Al,"logicSi1_al");
    G4Box *solidSi1_sub = new G4Box("solidSi1_sub",SiSize/2,SiSize/2,Si1subThickness/2);
    G4LogicalVolume *logicSi1_sub = new G4LogicalVolume(solidSi1_sub,Si,"logicSi1_sub");
    G4Box *solidSi1_epi = new G4Box("solidSi1_epi",SiSize/2,SiSize/2,Si1epiThickness/2);
    G4LogicalVolume *logicSi1_epi = new G4LogicalVolume(solidSi1_epi,Si,"logicSi1_epi");

    //fScoringVolume_si1_epi = logicSi1_epi;


    //si2(thick)
    G4double Si2nThickness = 1.5*um;
    G4double Si2intThickness = 675.*um;
    //G4double totalSi2Thickness = alThickness + Si2nThickness + Si2intThickness;
    G4double distance_from_si1 = alThickness + Si1subThickness + Si1epiThickness + gapThickness;
    G4double pos_si2 = pos_si1 + distance_from_si1;

    G4Box *solidSi2_al = new G4Box("solidSi2_al",SiSize/2,SiSize/2,alThickness/2);
    G4LogicalVolume *logicSi2_al = new G4LogicalVolume(solidSi2_al,Al,"logicSi2_al");
    G4Box *solidSi2_n = new G4Box("solidSi2_n",SiSize/2,SiSize/2,Si2nThickness/2);
    G4LogicalVolume *logicSi2_n = new G4LogicalVolume(solidSi2_n,Si,"logicSi2_n");
    G4Box *solidSi2_int = new G4Box("solidSi2_int",SiSize/2,SiSize/2,Si2intThickness/2);
    G4LogicalVolume *logicSi2_int = new G4LogicalVolume(solidSi2_int,Si,"logicSi2_int");

    //fScoringVolume_si2_int = logicSi2_int;

//신틸레이터 정의
    G4double CsISize = 2.*cm;
    G4double CsIThickness = 10.*cm;
    G4double pos_CsI = pos_si2 + 1.*cm;

    G4int ncomp = 2; //CsI = Cs+I
    G4double CsI_density = 4.51*g/cm3;
    G4Material*CsI = new G4Material("CsI", CsI_density, ncomp); //CsI material
    G4double a_Cs = 132.9*g/mole; // Cs(element) A
    G4int z_Cs = 55;           // Cs(element) Z
    G4Element*elCs = new G4Element("Cesium","Cs",z_Cs,a_Cs); // Cs(element)
    G4double a_I = 126*g/mole; //I(element) A
    G4int z_I = 53;        //I(element) Z
    G4Element*elI = new G4Element("Iodide","I",z_I,a_I); // I(element)
    G4int nAtoms;
    CsI->AddElement(elCs, nAtoms=1); //add element of by number of Cs
    CsI->AddElement(elI, nAtoms=1); //add element of by number of I

    G4Box *solid_CsI = new G4Box("solid_CsI",CsISize/2,CsISize/2,CsIThickness/2);
    G4LogicalVolume *logicCsI = new G4LogicalVolume(solid_CsI,CsI,"logicCsI");



   //copy no 
    G4int copyNo = 0;
    G4int copyNo_epi = 0;
    G4int copyNo_al1 = 0;
    G4int copyNo_sub = 0;
    G4int copyNo_n2 = 0;
    G4int copyNo_al2 = 0;
    G4int copyNo_int = 0;
    G4int copyNo_CsI = 0;





    // 회전 행렬 정의 
    G4RotationMatrix* rotX_minus180 = new G4RotationMatrix();
    rotX_minus180->rotateX(180. * deg); 
    G4RotationMatrix* rotX_90 = new G4RotationMatrix();
    rotX_90->rotateX(90. * deg);
    G4RotationMatrix* rotX_minus90 = new G4RotationMatrix();
    rotX_minus90->rotateX(-90. * deg);
    G4RotationMatrix* rotY_minus90 = new G4RotationMatrix();
    rotY_minus90->rotateY(-90. * deg);
    G4RotationMatrix* rotY_90 = new G4RotationMatrix();
    rotY_90->rotateY(90. * deg);



    return physWorld;
}
