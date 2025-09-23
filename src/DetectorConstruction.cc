// DetectorConstruction.cc
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

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  auto* nist = G4NistManager::Instance();

  // ========================= World =========================
  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
  auto* solidWorld = new G4Box("solidWorld", 3*m, 3*m, 3*m);
  auto* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  auto* physWorld  = new G4PVPlacement(
      nullptr, G4ThreeVector(0.,0.,0.), logicWorld,
      "phyWorld", nullptr, false, 0, true);

  // ========================= Target ========================
  G4Material* Au = nist->FindOrBuildMaterial("G4_Au");
  G4double target_size = 0.02*m;
  G4double target_Thickness = 6.*um;

  auto* solidTarget = new G4Box("solidTarget", target_size/2., target_size/2., target_Thickness/2.);
  auto* logicTarget = new G4LogicalVolume(solidTarget, Au, "logicTarget");
  (void) new G4PVPlacement(
      nullptr, G4ThreeVector(0.,0.,10.*cm), logicTarget,
      "physTarget", logicWorld, false, 0, true);

  auto* detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
  detVisAtt->SetForceSolid(true);
  logicTarget->SetVisAttributes(detVisAtt);

  // ========================= Materials / Sizes ========================
  G4double SiSize       = 2.*cm;
  G4double gapThickness = 2.*mm;
  G4double alThickness  = 1.*um;

  G4Material* Si = nist->FindOrBuildMaterial("G4_Si");
  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");

  // -- Si1 (thin)
  G4double Si1_subThickness = 15.*um;
  G4double Si1_epiThickness = 100.*um;
  G4double pos_si1          = 12.0*cm;   // "Si1 앞면" 기준 z

  auto* solidSi1_al  = new G4Box("solidSi1_al",  SiSize/2, SiSize/2, alThickness/2);
  auto* logicSi1_al  = new G4LogicalVolume(solidSi1_al,  Al, "logicSi1_al");

  auto* solidSi1_sub = new G4Box("solidSi1_sub", SiSize/2, SiSize/2, Si1_subThickness/2);
  auto* logicSi1_sub = new G4LogicalVolume(solidSi1_sub, Si, "logicSi1_sub");

  auto* solidSi1_epi = new G4Box("solidSi1_epi", SiSize/2, SiSize/2, Si1_epiThickness/2);
  auto* logicSi1_epi = new G4LogicalVolume(solidSi1_epi, Si, "logicSi1_epi");

  // -- Si2 (thick)
  G4double Si2_nThickness   = 1.5*um;
  G4double Si2_intThickness = 675.*um;

  G4double distance_from_si1 = alThickness + Si1_subThickness + Si1_epiThickness + gapThickness;
  G4double pos_si2           = pos_si1 + distance_from_si1;

  auto* solidSi2_al  = new G4Box("solidSi2_al",  SiSize/2, SiSize/2, alThickness/2);
  auto* logicSi2_al  = new G4LogicalVolume(solidSi2_al,  Al, "logicSi2_al");

  auto* solidSi2_n   = new G4Box("solidSi2_n",   SiSize/2, SiSize/2, Si2_nThickness/2);
  auto* logicSi2_n   = new G4LogicalVolume(solidSi2_n,   Si, "logicSi2_n");

  auto* solidSi2_int = new G4Box("solidSi2_int", SiSize/2, SiSize/2, Si2_intThickness/2);
  auto* logicSi2_int = new G4LogicalVolume(solidSi2_int, Si, "logicSi2_int");

  // -- CsI
  G4double CsISize      = 2.*cm;
  G4double CsIThickness = 10.*cm;
  G4double pos_CsI      = pos_si2 + 1.*cm;

  G4int ncomp = 2;
  G4double CsI_density = 4.51*g/cm3;
  auto* CsI_mat = new G4Material("CsI", CsI_density, ncomp);
  G4double a_Cs = 132.9*g/mole;  G4int z_Cs = 55;
  G4double a_I  = 126*g/mole;    G4int z_I  = 53;
  auto* elCs = new G4Element("Cesium","Cs", z_Cs, a_Cs);
  auto* elI  = new G4Element("Iodide","I", z_I, a_I);
  G4int nAtoms;
  CsI_mat->AddElement(elCs, nAtoms=1);
  CsI_mat->AddElement(elI,  nAtoms=1);

  auto* solid_CsI = new G4Box("solid_CsI", CsISize/2, CsISize/2, CsIThickness/2);
  auto* logicCsI  = new G4LogicalVolume(solid_CsI, CsI_mat, "logicCsI");

  // ========================= Telescope Assembly (Si1+Si2+CsI) =========================
  // 로컬 원점 z=0 을 "Si1 앞면"으로 두고, zacc로 뒤로 쌓아 올림
  G4AssemblyVolume* assemblyTelescope = new G4AssemblyVolume();
  G4RotationMatrix rotLocal; // no local rotation
  G4double zacc = 0.0;

  // Si1: Al
  {
    G4ThreeVector p(0.,0., zacc + 0.5*alThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi1_al, tr);
    zacc += alThickness;
  }
  // Si1: sub
  {
    G4ThreeVector p(0.,0., zacc + 0.5*Si1_subThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi1_sub, tr);
    zacc += Si1_subThickness;
  }
  // Si1: epi
  {
    G4ThreeVector p(0.,0., zacc + 0.5*Si1_epiThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi1_epi, tr);
    zacc += Si1_epiThickness;
  }
  // 공극: Si1 뒤 → Si2 앞
  zacc += (pos_si2 - pos_si1) - (alThickness + Si1_subThickness + Si1_epiThickness);

  // Si2: Al
  {
    G4ThreeVector p(0.,0., zacc + 0.5*alThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi2_al, tr);
    zacc += alThickness;
  }
  // Si2: n
  {
    G4ThreeVector p(0.,0., zacc + 0.5*Si2_nThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi2_n, tr);
    zacc += Si2_nThickness;
  }
  // Si2: int
  {
    G4ThreeVector p(0.,0., zacc + 0.5*Si2_intThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicSi2_int, tr);
    zacc += Si2_intThickness;
  }
  // 공극: Si2 뒤 → CsI 앞
  zacc += (pos_CsI - pos_si2) - (alThickness + Si2_nThickness + Si2_intThickness);

  // CsI
  {
    G4ThreeVector p(0.,0., zacc + 0.5*CsIThickness);
    G4Transform3D tr(rotLocal, p);
    assemblyTelescope->AddPlacedVolume(logicCsI, tr);
    zacc += CsIThickness;
  }

  // ========================= Block Assembly (4×4 Grid) =========================
  G4AssemblyVolume* assemblyBlock = new G4AssemblyVolume();

  const G4int NX = 4;
  const G4int NY = 4;

  // 텔레스코프 footprint와 간격
  G4double tileXY = (SiSize > CsISize ? SiSize : CsISize);
  G4double margin = 0.1*cm;
  G4double pitchX = tileXY + margin;
  G4double pitchY = tileXY + margin;

  // 블록 로컬에서 "격자 중심 = (0,0,0)"
  G4double x0 = -0.5 * (NX - 1) * pitchX;
  G4double y0 = -0.5 * (NY - 1) * pitchY;

  G4RotationMatrix rotNone; // 블록 내부에서 텔레스코프 별도의 회전 없음
  for (G4int iy=0; iy<NY; ++iy) {
    for (G4int ix=0; ix<NX; ++ix) {
      G4double x = x0 + ix*pitchX;
      G4double y = y0 + iy*pitchY;
      G4ThreeVector posInBlock(x, y, 0.); // telescope 로컬 z=0(=Si1 앞면)과 정렬

      G4Transform3D trTile(rotNone, posInBlock);
      assemblyBlock->AddPlacedAssembly(assemblyTelescope, trTile);
    }
  }

  // ========================= Place the Block into World =========================
  // 블록 전체를 y축 +40° 회전 & 블록 "앞면 중심(=블록 로컬 원점)"을 (1m*sin40°, 0, 1m*cos40°) 로 이동
  G4RotationMatrix rotBlock;
  //rotBlock.rotateY(40.*deg);

 // G4ThreeVector blockCenter( 1.*m * std::sin(40.*deg),
 //                            0.,
 //                            1.*m * std::cos(40.*deg) );
   G4ThreeVector blockCenter(0., 0., 1.*m); 
  const G4int  baseCopyNoBlock = 1000;  
  const G4bool checkOverlap    = true;

  G4Transform3D trBlock(rotBlock, blockCenter);
  assemblyBlock->MakeImprint(logicWorld, trBlock, baseCopyNoBlock, checkOverlap);

  return physWorld;
}

