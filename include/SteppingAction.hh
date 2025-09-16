#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "EventAction.hh"

using namespace CLHEP;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    G4AnalysisManager* analysisManager;

    EventAction* eventAction = nullptr;
};

#endif
