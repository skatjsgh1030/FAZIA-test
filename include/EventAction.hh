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

  private:
    EventAction & operator=(const EventAction &right);
    EventAction(const EventAction&);
};
#endif
