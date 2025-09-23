#include "EventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4DataInterpolation.hh"

// 기존엔 주석 처리되어 있었던 분석 매니저 헤더를 활성화
#include "G4AnalysisManager.hh"     // G4AnalysisManager
#include "G4String.hh"    // G4String
#include <unordered_map> // extern 맵 선언용

using namespace CLHEP;

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  // 필요 시 이벤트 시작 시점 초기화 로직 추가 가능 (현재는 비움)
  // 매 이벤트 시작 때 마다 gold target event를 initialize 해준다.
  isGoldTarget = false;

  edepSi1 = 0;
  edepSi2 = 0;
  edepCsI = 0;
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // 이벤트 종료 시: 각 트랙이 각 검출기에서 잃은 총합을 한 줄로 기록
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int eventID = evt->GetEventID();


  analysisManager->FillNtupleIColumn(1, 0, eventID);
  analysisManager->FillNtupleDColumn(1, 1, edepSi1);
  analysisManager->FillNtupleDColumn(1, 2, edepSi2);
  analysisManager->FillNtupleDColumn(1, 3, edepCsI);
  analysisManager->AddNtupleRow(1);
}

