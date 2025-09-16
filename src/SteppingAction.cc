#include "SteppingAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"

// ===== [추가] 누적용 컨테이너/헤더 =====
#include <unordered_map>
#include "G4VProcess.hh"
#include "G4TouchableHistory.hh" // G4TouchableHandle 사용 시 안전
// 트랙별 총 에너지 손실 누적 맵 (검출기별)
std::unordered_map<G4int, G4double> map_trackId_to_edep_Si1_total; // Si1 = logicSi1_sub + logicSi1_epi
std::unordered_map<G4int, G4double> map_trackId_to_edep_Si2_total; // Si2 = logicSi2_n   + logicSi2_int
std::unordered_map<G4int, G4double> map_trackId_to_edep_CsI_total; // CsI = logicCsI
// 트랙 메타데이터 (한 번만 저장)
std::unordered_map<G4int, G4int>    map_trackId_to_pdg_code;
std::unordered_map<G4int, G4String> map_trackId_to_creator_process_name;
// =====================================

SteppingAction::SteppingAction()
  : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  analysisManager = G4AnalysisManager::Instance();

  // Saving Data (기존 변수들 유지)
  G4int eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
  //G4int trackID = step -> GetTrack() -> GetTrackID();
  G4int parentID = step -> GetTrack() -> GetParentID();
  //G4int volumeID = step -> GetTrack() -> GetVolume() -> GetCopyNo();
  G4int pdg = step -> GetTrack() -> GetDefinition() -> GetPDGEncoding();
  G4double edep = step -> GetTotalEnergyDeposit();




  // =====각 트랙이 각 검출기에서 잃은 에너지 누적 추가=====
  // 더 명확한 변수명
  G4int    current_track_id    = step->GetTrack()->GetTrackID();
  G4int    current_pdg_code    = pdg; // 위에서 얻은 pdg 재사용
  G4double energy_deposit_step = edep;

  // 스텝 시작점에서 현재 물리/논리 볼륨 얻기
  const G4StepPoint* pre_step_point   = step->GetPreStepPoint();
  G4TouchableHandle  touchable_handle = pre_step_point->GetTouchableHandle();
  G4VPhysicalVolume* current_phys_volume = touchable_handle->GetVolume();  // null 가능
  if (current_phys_volume == 0 || energy_deposit_step <= 0.0) return;



  if (current_phys_volume != 0 && energy_deposit_step > 0.0) {
    G4LogicalVolume* current_logical_volume = current_phys_volume->GetLogicalVolume();
    G4String         current_logical_name   = current_logical_volume->GetName();

  if (parentID > 0) {
    // 트랙 메타데이터(PDG, 생성 프로세스) 최초 1회 기록
    if (map_trackId_to_pdg_code.find(current_track_id) == map_trackId_to_pdg_code.end()) {
      map_trackId_to_pdg_code[current_track_id] = current_pdg_code;
      const G4VProcess* creator_process_ptr = step->GetTrack()->GetCreatorProcess();
      G4String creator_process_name = (creator_process_ptr ? creator_process_ptr->GetProcessName() : "unknown");
      map_trackId_to_creator_process_name[current_track_id] = creator_process_name;
    }

    // DetectorConstruction에서 정의된 논리볼륨 이름으로 판별 후 누적
    // Si1: logicSi1_sub, logicSi1_epi / Si2: logicSi2_n, logicSi2_int / CsI: logicCsI :contentReference[oaicite:0]{index=0}
    if (current_logical_name == "logicSi1_epi") {//if (current_logical_name == "logicSi1_sub" || current_logical_name == "logicSi1_epi") <- 복수 볼륨 사
      map_trackId_to_edep_Si1_total[current_track_id] += energy_deposit_step;
    } else if (current_logical_name == "logicSi2_int") {
      map_trackId_to_edep_Si2_total[current_track_id] += energy_deposit_step;
    } else if (current_logical_name == "logicCsI") {
      map_trackId_to_edep_CsI_total[current_track_id] += energy_deposit_step;
    }
  }
} 
  // =================================================================




  // 내가 처음 쏴주는 양성자일때 
  if( parentID==0 && pdg==2212 )
  {
    const std::vector< const G4Track* >* secondaryTracks = step->GetSecondaryInCurrentStep();
    int numOfSecondary = secondaryTracks->size();
  

    if( numOfSecondary!=0 )
    {
      const G4Track* secondTrack = secondaryTracks->at(0);
      auto proc = secondTrack->GetCreatorProcess();
      G4String processName = proc->GetProcessName();
      //G4cout << processName << G4endl;
      //G4cout << numOfSecondary << G4endl;



      if( processName=="hadElastic" )
      {
        // proton의 에너지와 secondary의 저장
        // 트랙이 하나만 생성 됐으니까, 지금 proton 과 생성된 secondary 하나만 저장
        G4ThreeVector protonMom = step->GetTrack()->GetMomentum();
        G4ThreeVector targetMom = secondTrack->GetMomentum();

        G4double protonMass = step->GetTrack()->GetDefinition()->GetPDGMass();
        G4double targetMass = secondTrack->GetDefinition()->GetPDGMass();
        G4double protonKe = step->GetTrack()->GetKineticEnergy();
        G4double targetKe = secondTrack->GetKineticEnergy();

        analysisManager->FillNtupleIColumn( 0, eventID );
        analysisManager->FillNtupleDColumn( 1, 2212 );
        analysisManager->FillNtupleDColumn( 2, protonMom.x() );
        analysisManager->FillNtupleDColumn( 3, protonMom.y() );
        analysisManager->FillNtupleDColumn( 4, protonMom.z() );
        analysisManager->FillNtupleDColumn( 5, protonMass );
        analysisManager->FillNtupleDColumn( 6, protonKe );
        analysisManager -> AddNtupleRow();


        analysisManager->FillNtupleIColumn( 0, eventID );
        analysisManager->FillNtupleDColumn( 1, secondTrack->GetDefinition()->GetPDGEncoding() );
        analysisManager->FillNtupleDColumn( 2, targetMom.x() );
        analysisManager->FillNtupleDColumn( 3, targetMom.y() );
        analysisManager->FillNtupleDColumn( 4, targetMom.z() );
        analysisManager->FillNtupleDColumn( 5, targetMass );
        analysisManager->FillNtupleDColumn( 6, targetKe );
        analysisManager -> AddNtupleRow();
      }

      if( processName=="protonInelastic" )
      {
        // secondary 나오는 트랙들의 정보 저장
        for( G4int i=0; i<numOfSecondary; i++ )
        {
          const G4Track* daughterTrack = secondaryTracks->at(i);
          G4ThreeVector daughterMom = daughterTrack->GetMomentum();
          G4double daughterMass = daughterTrack->GetDefinition()->GetPDGMass();
          G4double ke = daughterTrack->GetKineticEnergy();
          //G4int daughterPDG = daughterTrack ->GetDefinition()->GetPDGEncoding(); 
          //G4cout << daughterPDG << G4endl;
          analysisManager->FillNtupleIColumn( 0, eventID );
          analysisManager->FillNtupleDColumn( 1, daughterTrack->GetDefinition()->GetPDGEncoding() );
          analysisManager->FillNtupleDColumn( 2, daughterMom.x() );
          analysisManager->FillNtupleDColumn( 3, daughterMom.y() );
          analysisManager->FillNtupleDColumn( 4, daughterMom.z() );
          analysisManager->FillNtupleDColumn( 5, daughterMass );
          analysisManager->FillNtupleDColumn( 6, ke );
          analysisManager -> AddNtupleRow();
        }
      }
      if( processName=="CoulombScat" )
      {
        // secondary 나오는 트랙들의 정보 저장
        for( G4int i=0; i<numOfSecondary; i++ )
        {
          const G4Track* daughterTrack = secondaryTracks->at(i);
          G4ThreeVector daughterMom = daughterTrack->GetMomentum();
          G4double daughterMass = daughterTrack->GetDefinition()->GetPDGMass();
          G4double ke = daughterTrack->GetKineticEnergy();
          //G4int daughterPDG = daughterTrack ->GetDefinition()->GetPDGEncoding(); 
          //G4cout << daughterPDG << G4endl;
          analysisManager->FillNtupleIColumn( 0, eventID );
          analysisManager->FillNtupleDColumn( 1, daughterTrack->GetDefinition()->GetPDGEncoding() );
          analysisManager->FillNtupleDColumn( 2, daughterMom.x() );
          analysisManager->FillNtupleDColumn( 3, daughterMom.y() );
          analysisManager->FillNtupleDColumn( 4, daughterMom.z() );
          analysisManager->FillNtupleDColumn( 5, daughterMass );
          analysisManager->FillNtupleDColumn( 6, ke );
          analysisManager -> AddNtupleRow();
        }
      }

    }
  }

}

