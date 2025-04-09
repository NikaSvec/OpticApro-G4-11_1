// TrackingAction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "EventAction.hh"

#include "globals.hh"
#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "vector"

class EventAction;
class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction
{
 public:
  TrackingAction(EventAction*);
  ~TrackingAction();

  void PreUserTrackingAction(const G4Track*) override;

 private:
  EventAction* fEventAction;
  G4double fEdep;
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
