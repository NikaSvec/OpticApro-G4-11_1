// SteppingAction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "EventAction.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4UserSteppingAction.hh"
#include "vector"
#include "G4SteppingVerbose.hh"
#include "G4SteppingManager.hh"

class EventAction;
class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
 public:
  SteppingAction(EventAction*);
  ~SteppingAction();

  void UserSteppingAction(const G4Step*) override;

 private:
  EventAction* fEventAction;
  G4double fEdep;
  
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
