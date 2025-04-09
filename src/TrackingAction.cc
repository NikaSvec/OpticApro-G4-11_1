// TrackingAction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "Run.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4TouchableHandle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"

#include "fstream"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
  : G4UserTrackingAction()
  , fEventAction(event)
{
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    const G4Event *evt = G4RunManager::GetRunManager()->GetCurrentEvent();
    auto runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    std::ofstream& csvFile = runAction->GetCsvFile();

    // energy deposit
    fEdep = 0.0; //No energy deposit in Step 0

    G4TouchableHandle handle = track->GetTouchableHandle();
    G4String volumeName = handle->GetVolume()->GetName();

    //if(volumeName == "Envelope") //when we want to register particles only in particular volume (i.e. envelope), uncomment if
      //{
          int EventID = evt->GetEventID();  //EventID
          int TrackID = track->GetTrackID();  //trackID
          int StepNum = track->GetCurrentStepNumber();  //step number
          int ParentID = track->GetParentID();  //parentID
          int DaughterPDG = track->GetDynamicParticle()->GetDefinition()->GetPDGEncoding(); //daughter PDG
          double KinEn = track->GetKineticEnergy(); //kin. energy [MeV]
          double DirX = track->GetPosition().x(); //direction x [cm] (origin: center of the world volume)
          double DirY = track->GetPosition().y(); //direction y [cm] (origin: center of the world volume)
          double DirZ = track->GetPosition().z(); //direction z [cm] (origin: center of the world volume)
          double MomX = track->GetDynamicParticle()->GetMomentum().x();  //momentum x [MeV/c]
          double MomY = track->GetDynamicParticle()->GetMomentum().y();  //momentum y [MeV/c]
          double MomZ = track->GetDynamicParticle()->GetMomentum().z();  //momentum z [MeV/c]
          double MomTot = track->GetDynamicParticle()->GetTotalMomentum();  //momentum Total [MeV/c]
          double Speed = track->GetVelocity(); //velocity [cm/s]
          double Volume;

          if(volumeName == "World")
            {
              Volume = 1;
            }
          else if(volumeName == "Envelope")
            {
              Volume = 2;
            }
          else
            {
              Volume = 0;
            }

          int Process;

          if(ParentID != 0)
            {
              Process = track->GetCreatorProcess()->GetProcessType()*1000 + track->GetCreatorProcess()->GetProcessSubType();
            }
          else
            {
              Process = 1000;
            }

          fEventAction->AddStepData(EventID, TrackID, StepNum, ParentID, DaughterPDG, KinEn, DirX, DirY, DirZ, MomX, MomY, MomZ, MomTot, Speed, Volume, Process);

          if(csvFile.is_open())
            {
              csvFile << EventID << "," << TrackID << "," << StepNum << "," << ParentID << "," << DaughterPDG << "," << KinEn / MeV << "," << DirX / cm << "," << DirY / cm << "," << DirZ / cm << "," << MomX / (MeV / c_light) << "," << MomY / (MeV / c_light) << "," << MomZ / (MeV / c_light) << "," << MomTot / (MeV / c_light) << "," << Speed / (cm / s) << "," << fEdep / MeV << "," << Volume << "," << Process << "\n";
            }
      //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
