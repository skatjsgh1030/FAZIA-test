#include <ctime>
#include <cstdlib>
#include <unistd.h>        // getpid
#include <string>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"      // Qt 세션
#include "G4VisExecutive.hh"
#include "Randomize.hh"
#include "CLHEP/Random/MTwistEngine.h"

// 사용자 헤더(프로젝트에 맞게 유지)
#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

int main(int argc, char** argv)
{
  // --------- 0) 입력 인자 처리 (세그폴트 방지) ----------
  // argv[1] : 파일 넘버/시드용 숫자(옵션)
  // argv[2] : 매크로 파일(옵션)
  int ranSeed = 1;
  G4String fileNumber = "0001";
  if (argc >= 2) {
    ranSeed = std::max(1, atoi(argv[1]));     // 0 방지
    fileNumber = argv[1];
  }

  // --------- 1) 안전한 랜덤 엔진/시드 설정 ----------
  // RunManager 생성 "전에" 설정!
  auto* engine = new CLHEP::MTwistEngine();
  G4Random::setTheEngine(engine);

  const long t = static_cast<long>(time(nullptr));
  long seeds[2];
  seeds[0] = t ^ (0x9e3779b9L * ranSeed);
  seeds[1] = static_cast<long>(getpid());
  G4Random::setTheSeeds(seeds, 2);

  // --------- 2) UI 세션을 가장 먼저 준비 (인터랙티브일 때) ----------
  // 인자가 2개 미만이면 인터랙티브로 보고 Qt 세션 생성
  G4UIExecutive* ui = nullptr;
  if (argc <= 2) {
    ui = new G4UIExecutive(argc, argv, "Qt"); // Qt 힌트 (선택)
  }

  // --------- 3) RunManager/유저 초기화 ----------
  auto* runManager = new G4RunManager;
  G4bool bSaveKinematics = true;
  G4double kinEnergy = 70.*CLHEP::MeV;
  G4double rotationDeg = 30.;

  G4String deg = G4UIcommand::ConvertToString((int)rotationDeg);

  runManager->SetUserInitialization(new DetectorConstruction(rotationDeg));
  runManager->SetUserInitialization(new FTFP_BERT());
  runManager->SetUserAction(new RunAction(fileNumber, deg,bSaveKinematics));
  runManager->SetUserAction(new PrimaryGeneratorAction(kinEnergy));
  runManager->SetUserAction(new EventAction());
  runManager->SetUserAction(new SteppingAction(bSaveKinematics));
  runManager->Initialize();

  // --------- 4) Vis 매니저 (UI 세션 이후에!) ----------
  auto* visManager = new G4VisExecutive;
  visManager->Initialize();

  auto* UImanager = G4UImanager::GetUIpointer();

  // --------- 5) 매크로/실행 분기 ----------
  if (ui) {
    // 인터랙티브: Qt 드라이버 사용 가능 (vis.mac 안에서 /vis/open OGLSQt 사용)
    UImanager->ApplyCommand("/control/execute vis_1.mac");
    ui->SessionStart();
    delete ui;
  } else {
    // 배치: 사용자 매크로 실행 (배치용 vis 드라이버를 매크로에서 선택)
    if (argc >= 3) {
      G4String macro = argv[2];
      UImanager->ApplyCommand("/control/execute " + macro);
    }
  }

  // --------- 6) 정리 ----------
  delete visManager;
  delete runManager;
  return 0;
}

