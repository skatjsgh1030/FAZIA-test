#include "SteppingAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"

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

  // Saving Data
  G4int eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
  //G4int trackID = step -> GetTrack() -> GetTrackID();
  G4int parentID = step -> GetTrack() -> GetParentID();
  //G4int volumeID = step -> GetTrack() -> GetVolume() -> GetCopyNo();
  G4int pdg = step -> GetTrack() -> GetDefinition() -> GetPDGEncoding();
  G4double edep = step -> GetTotalEnergyDeposit();



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
