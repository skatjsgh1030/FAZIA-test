#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"

#include "G4AnalysisManager.hh"

class RunAction : public G4UserRunAction
{
  public:
    RunAction( G4String fileN, G4String deteDegree ,G4bool bSaveKinematics );
    virtual ~RunAction();

    // method from the base class
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    private:
    G4bool fSaveKinematics = true;
};

#endif
