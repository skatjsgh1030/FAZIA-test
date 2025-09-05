#include "RunAction.hh"
#include "EventAction.hh"
#include "G4EventManager.hh"

#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
  :G4UserRunAction()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String fileName = "output.root";
  analysisManager->OpenFile(fileName);

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(0); // default is 0
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run* )
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->CreateNtuple("event", "");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("pid");
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
  analysisManager->CreateNtupleDColumn("mass");
  analysisManager->CreateNtupleDColumn("ke");
  analysisManager->FinishNtuple(0);
}

void RunAction::EndOfRunAction(const G4Run* theRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
