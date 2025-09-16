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

  private:
    EventAction & operator=(const EventAction &right);
    EventAction(const EventAction&);

    G4bool isGoldTarget = false;
};
#endif
