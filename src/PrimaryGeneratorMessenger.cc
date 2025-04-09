// PrimaryGeneratorMessenger source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* OptAproGun)
  : G4UImessenger()
  , fOptAproAction(OptAproGun)
{
  fGunDir = new G4UIdirectory("/OpticApro-G4-11_1/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fPolarCmd =
    new G4UIcmdWithADoubleAndUnit("/OpticApro-G4-11_1/gun/optPhotonPolar", this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle", true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                                    G4String newValue)
{
  if(command == fPolarCmd)
  {
    G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
    if(angle == -360.0 * deg)
    {
      fOptAproAction->SetOptPhotonPolar();
    }
    else
    {
      fOptAproAction->SetOptPhotonPolar(angle);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
