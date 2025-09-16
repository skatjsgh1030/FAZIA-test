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
    G4double target_size = 0.02*m;
    G4double target_Thickness = 6.*um;     
    G4Box *solidTarget = new G4Box("solidTarget",target_size/2.,target_size/2.,target_Thickness/2.);
    G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget,Au,"logicTarget");
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
    G4double Si1_subThickness = 15.*um;
    G4double Si1_epiThickness = 100.*um;
    //G4double totalSi1Thickness = alThickness + Si1subThickness + Si1epiThickness;
    G4double pos_si1 = 12.0*cm; 

    G4Box *solidSi1_al = new G4Box("solidSi1_al",SiSize/2,SiSize/2,alThickness/2);
    G4LogicalVolume *logicSi1_al = new G4LogicalVolume(solidSi1_al,Al,"logicSi1_al");
    G4Box *solidSi1_sub = new G4Box("solidSi1_sub",SiSize/2,SiSize/2,Si1_subThickness/2);
    G4LogicalVolume *logicSi1_sub = new G4LogicalVolume(solidSi1_sub,Si,"logicSi1_sub");
    G4Box *solidSi1_epi = new G4Box("solidSi1_epi",SiSize/2,SiSize/2,Si1_epiThickness/2);
    G4LogicalVolume *logicSi1_epi = new G4LogicalVolume(solidSi1_epi,Si,"logicSi1_epi");

    //fScoringVolume_si1_epi = logicSi1_epi;


    //si2(thick)
    G4double Si2_nThickness = 1.5*um;
    G4double Si2_intThickness = 675.*um;
    //G4double totalSi2Thickness = alThickness + Si2nThickness + Si2intThickness;
    G4double distance_from_si1 = alThickness + Si1_subThickness + Si1_epiThickness + gapThickness;
    G4double pos_si2 = pos_si1 + distance_from_si1;

    G4Box *solidSi2_al = new G4Box("solidSi2_al",SiSize/2,SiSize/2,alThickness/2);
    G4LogicalVolume *logicSi2_al = new G4LogicalVolume(solidSi2_al,Al,"logicSi2_al");
    G4Box *solidSi2_n = new G4Box("solidSi2_n",SiSize/2,SiSize/2,Si2_nThickness/2);
    G4LogicalVolume *logicSi2_n = new G4LogicalVolume(solidSi2_n,Si,"logicSi2_n");
    G4Box *solidSi2_int = new G4Box("solidSi2_int",SiSize/2,SiSize/2,Si2_intThickness/2);
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
/*    
    //Si1 (115um) 배치
    G4VPhysicalVolume *phySi1_al = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si1 + alThickness/2),logicSi1_al,"phySi1_al",logicWorld, false, 0, true);
    G4VPhysicalVolume *phySi1_sub = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si1 + alThickness + Si1_subThickness/2),logicSi1_sub,"phySi1_sub",logicWorld, false, 0, true);
    G4VPhysicalVolume *physSi1_epi = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si1 + alThickness + Si1_subThickness + Si1_epiThickness/2),logicSi1_epi,"phySi1_al",logicWorld, false, 0, true);

    //Si2 (675um) 배치
    G4VPhysicalVolume *physSi2_al = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si2 + alThickness/2),logicSi2_al,"phySi2_al",logicWorld, false, 0, true);
    G4VPhysicalVolume *physSi2_n = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si2 + alThickness + Si2_nThickness/2),logicSi2_n,"phySi2_n",logicWorld, false, 0, true);
    G4VPhysicalVolume *physSi2_int = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_si2 + alThickness + Si2_nThickness + Si2_intThickness/2),logicSi2_int,"phySi2_int",logicWorld, false, 0, true);

    //CsI 배치
    G4VPhysicalVolume *physCsI = new G4PVPlacement(0,G4ThreeVector(0., 0., pos_CsI + CsIThickness/2),logicCsI,"phyCsI",logicWorld, false, 0, true);
*/



    // =================== [추가] Telescope( Si1 + Si2 + CsI ) 어셈블리 구성 및 배치 ===================
    {
      // (1) 어셈블리 생성: 로컬 원점 z=0을 "Si1 앞면"으로 가정
      G4AssemblyVolume* assemblyTelescope = new G4AssemblyVolume();
      G4RotationMatrix  rotLocal;   // 로컬 회전(없음)
      G4double          zacc = 0.0; // 로컬 z 누적자(앞→뒤로 쌓기)

      // (2) Si1 stack: Al → sub → epi (각 층 '센터' 배치 → 0.5*두께)
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*alThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi1_al, tr);
        zacc += alThickness;
      }
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*Si1_subThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi1_sub, tr);
        zacc += Si1_subThickness;
      }
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*Si1_epiThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi1_epi, tr);
        zacc += Si1_epiThickness;
      }

      // (3) Si1 뒤면 → Si2 앞면 공극: (pos_si2 - pos_si1) - (Si1 전체 두께)
      zacc += (pos_si2 - pos_si1) - (alThickness + Si1_subThickness + Si1_epiThickness);

      // (4) Si2 stack: Al → n → int
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*alThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi2_al, tr);
        zacc += alThickness;
      }
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*Si2_nThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi2_n, tr);
        zacc += Si2_nThickness;
      }
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*Si2_intThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicSi2_int, tr);
        zacc += Si2_intThickness;
      }

      // (5) Si2 뒤면 → CsI 앞면 공극: (pos_CsI - pos_si2) - (Si2 전체 두께)
      zacc += (pos_CsI - pos_si2) - (alThickness + Si2_nThickness + Si2_intThickness);

      // (6) CsI
      {
        G4ThreeVector posLocal(0., 0., zacc + 0.5*CsIThickness);
        G4Transform3D tr(rotLocal, posLocal);
        assemblyTelescope->AddPlacedVolume(logicCsI, tr);
        zacc += CsIThickness;
      }

      // (7) 월드에 인쇄(MakeImprint): 로컬 원점(=Si1 앞면)을 월드의 pos_si1로 이동

    // =================== [추가] 격자(Grid) 형태로 여러 개 배치 (복사번호 매핑 포함) ==================
      {
        // ---- 회전(옵션): 텔레스코프 축을 z축에 수직으로 두고 싶으면 아래 한 줄 주석 해제 ----
        G4RotationMatrix rotWorld;
        //rotWorld.rotateY(20.*deg);  // 로컬 z(스택 방향)를 월드 x로 회전 → z에 수직

        // ---- 격자 크기 (열×행) ----
        const G4int NX = 4;   // 가로 개수
        const G4int NY = 4;   // 세로 개수

        // ---- 피치(간격): 가장 넓은 가로 폭 + 여유 ----
        G4double tileXY = (SiSize > CsISize ? SiSize : CsISize);
        G4double margin = 0.1*cm;
        G4double pitchX = tileXY + margin;   // X방향 간격
        G4double pitchY = tileXY + margin;   // Y방향 간격

        // ---- 격자 중심 정렬용 시작 좌표 (0,0) 기준 대칭 배치 ----
        G4double x0 = -0.5 * (NX - 1) * pitchX;
        G4double y0 = -0.5 * (NY - 1) * pitchY;
        G4double z0 = pos_si1;               // Si1 앞면 z는 기존과 동일

        // ---- 복사번호 전략: 텔레스코프마다 넉넉히 띄움 ----
        // 내부 컴포넌트 offset(어셈블리 등록 순서):
        //   0:Si1_al, 1:Si1_sub, 2:Si1_epi, 3:Si2_al, 4:Si2_n, 5:Si2_int, 6:CsI
        const G4int copy_stride = 100;  // (≥7 권장)

        for (G4int iy = 0; iy < NY; ++iy) {
          for (G4int ix = 0; ix < NX; ++ix) {

            // 위치 계산
            G4double x = x0 + ix * pitchX;
            G4double y = y0 + iy * pitchY;
            G4ThreeVector posWorld(x, y, z0);

            // base copy 번호 (iy*100 + ix) * COPY_STRIDE 형태 — NX<100 가정 시 깔끔
            G4int baseCopyNo = (iy * copy_stride + ix) * copy_stride;

            // ★ 임시(rvalue)가 아니라 지역 변수(lvalue)로 변환 생성
            G4Transform3D trWorld(rotWorld, posWorld);

            G4bool checkOverlap = true;  // 개발 중 true 권장
            assemblyTelescope->MakeImprint(
                logicWorld,
                trWorld,          // ← lvalue 전달 (rvalue 금지)
                baseCopyNo,
                checkOverlap
            );
          }
        }
      } // ← 격자 블록 닫기
    }   // ← 어셈블리 구성 블록 닫기

    return physWorld;
} // ← Construct() 닫기
