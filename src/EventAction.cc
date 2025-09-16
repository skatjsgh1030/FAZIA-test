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

// ===== SteppingAction.cc에서 정의된 전역 누적 컨테이너를 참조(extern) =====
extern std::unordered_map<G4int, G4double> map_trackId_to_edep_Si1_total;
extern std::unordered_map<G4int, G4double> map_trackId_to_edep_Si2_total;
extern std::unordered_map<G4int, G4double> map_trackId_to_edep_CsI_total;

extern std::unordered_map<G4int, G4int>    map_trackId_to_pdg_code;
extern std::unordered_map<G4int, G4String> map_trackId_to_creator_process_name;
// ========================================================================

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  // 필요 시 이벤트 시작 시점 초기화 로직 추가 가능 (현재는 비움)
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // 이벤트 종료 시: 각 트랙이 각 검출기에서 잃은 총합을 한 줄로 기록
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int current_event_id = evt->GetEventID();

  // 트랙 식별은 PDG 맵의 key(trackID)를 기준으로 순회
  for (std::unordered_map<G4int, G4int>::iterator it = map_trackId_to_pdg_code.begin();
       it != map_trackId_to_pdg_code.end(); ++it)
  {
    G4int    track_id              = it->first;
    G4int    pdg_code              = it->second;

    G4double edep_Si1_total = 0.0;
    G4double edep_Si2_total = 0.0;
    G4double edep_CsI_total = 0.0;

    std::unordered_map<G4int, G4double>::iterator f1 = map_trackId_to_edep_Si1_total.find(track_id);
    if (f1 != map_trackId_to_edep_Si1_total.end()) { edep_Si1_total = f1->second; }

    std::unordered_map<G4int, G4double>::iterator f2 = map_trackId_to_edep_Si2_total.find(track_id);
    if (f2 != map_trackId_to_edep_Si2_total.end()) { edep_Si2_total = f2->second; }

    std::unordered_map<G4int, G4double>::iterator f3 = map_trackId_to_edep_CsI_total.find(track_id);
    if (f3 != map_trackId_to_edep_CsI_total.end()) { edep_CsI_total = f3->second; }

    // 세 검출기 중 하나라도 에너지 손실이 있으면 기록
    if (edep_Si1_total > 0.0 || edep_Si2_total > 0.0 || edep_CsI_total > 0.0)
    {
      G4String creator_process_str = "unknown";
      std::unordered_map<G4int, G4String>::iterator fcp = map_trackId_to_creator_process_name.find(track_id);
      if (fcp != map_trackId_to_creator_process_name.end()) { creator_process_str = fcp->second; }

      // Ntuple "edep_by_track" (ID=1) 컬럼 순서:
      // [0]=eventID, [1]=trackID, [2]=pdg, [3]=creator_process,
      // [4]=edep_Si1_total, [5]=edep_Si2_total, [6]=edep_CsI_total
      analysisManager->FillNtupleIColumn(1, 0, current_event_id);
      analysisManager->FillNtupleIColumn(1, 1, track_id);
      analysisManager->FillNtupleIColumn(1, 2, pdg_code);
      analysisManager->FillNtupleSColumn(1, 3, creator_process_str);
      analysisManager->FillNtupleDColumn(1, 4, edep_Si1_total);
      analysisManager->FillNtupleDColumn(1, 5, edep_Si2_total);
      analysisManager->FillNtupleDColumn(1, 6, edep_CsI_total);
      analysisManager->AddNtupleRow(1);
    }
  }

  // 다음 이벤트 대비: 누적 컨테이너 초기화
  map_trackId_to_edep_Si1_total.clear();
  map_trackId_to_edep_Si2_total.clear();
  map_trackId_to_edep_CsI_total.clear();
  map_trackId_to_pdg_code.clear();
  map_trackId_to_creator_process_name.clear();
}

