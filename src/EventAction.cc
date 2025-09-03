#include "EventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4DataInterpolation.hh"

//#include "g4root.hh"

using namespace CLHEP;
EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
}
