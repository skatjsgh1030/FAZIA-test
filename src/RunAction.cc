#include "RunAction.hh"
#include "EventAction.hh"
#include "G4EventManager.hh"

#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction( G4bool bSaveKinematics )
  :G4UserRunAction()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String fileName = "output.root";
  analysisManager->OpenFile(fileName);

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(0); // default is 0

  fSaveKinematics = bSaveKinematics;
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run* )
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->CreateNtuple("event_kine", "");
  if( fSaveKinematics )
  {
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("targetZ");
  analysisManager->CreateNtupleIColumn("targetA");
  analysisManager->CreateNtupleDColumn("pid");
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
  analysisManager->CreateNtupleDColumn("mass");
  analysisManager->CreateNtupleDColumn("ke");
  }
  analysisManager->FinishNtuple(0);

  //Energy deposit정보 저장
  analysisManager->CreateNtuple("event_edep", "");
  analysisManager->CreateNtupleIColumn("eventID");
  for( G4int i=0; i<16; i++ )
  {
    G4String telNum = G4UIcommand::ConvertToString(i);
    analysisManager->CreateNtupleDColumn("edep_Si1_"+telNum);
    analysisManager->CreateNtupleDColumn("edep_Si2_"+telNum);
    analysisManager->CreateNtupleDColumn("edep_CsI_"+telNum);
  }
  analysisManager->FinishNtuple(1);


}

void RunAction::EndOfRunAction(const G4Run* theRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
