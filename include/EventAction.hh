#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
//#include "g4root.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
  public: // Without description

    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);

    void SetGoldTarget( G4bool val ) { isGoldTarget = val; };
    G4bool GetGoldTarget() { return isGoldTarget; };


    void AccumulateEdepSi1( G4int det, G4double val ) { edepSi1[det] += val; };
    void AccumulateEdepSi2( G4int det, G4double val ) { edepSi2[det] += val; };
    void AccumulateEdepCsI( G4int det, G4double val ) { edepCsI[det] += val; };

  private:
    EventAction & operator=(const EventAction &right);
    EventAction(const EventAction&);

    G4bool isGoldTarget = false;

    G4double edepSi1[16] = {0};
    G4double edepSi2[16] = {0};
    G4double edepCsI[16] = {0};
};
#endif
