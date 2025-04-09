// SteppingAction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "Run.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
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

SteppingAction::SteppingAction(EventAction* event)
  : G4UserSteppingAction()
  , fEventAction(event)
{
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  static G4ParticleDefinition* opticalphoton =
    G4OpticalPhoton::OpticalPhotonDefinition();

  const G4ParticleDefinition* particleDef =
    step->GetTrack()->GetDynamicParticle()->GetParticleDefinition();

  if(particleDef == opticalphoton)
  {
    G4StepPoint* endPoint = step->GetPostStepPoint();
    const G4VProcess* pds = endPoint->GetProcessDefinedStep();
    G4String procname     = pds->GetProcessName();
    if(procname.compare("OpRayleigh") == 0)
      fEventAction->AddRayleigh();
    else if(procname.compare("OpAbsorption") == 0)
      fEventAction->AddAbsorption();
    else if(procname.compare("OpMieHG") == 0)
      fEventAction->AddMie();

    // for boundary scattering, process name in 'transportation'.
    // Need to check differently:
    if(endPoint->GetStepStatus() == fGeomBoundary)
    {
      G4OpBoundaryProcessStatus theStatus = Undefined;
      G4ProcessManager* opManager         = opticalphoton->GetProcessManager();
      G4int n_proc = opManager->GetPostStepProcessVector(typeDoIt)->entries();
      G4ProcessVector* postStepDoItVector =
        opManager->GetPostStepProcessVector(typeDoIt);
      for(G4int i = 0; i < n_proc; ++i)
      {
        G4VProcess* currentProcess = (*postStepDoItVector)[i];

        G4OpBoundaryProcess* opProc =
          dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
        if(opProc)
          theStatus = opProc->GetStatus();
      }
      if(theStatus != Undefined && theStatus != NotAtBoundary &&
         theStatus != StepTooSmall)
      {
        fEventAction->AddBoundary();
      }
    }
  }

  const G4Event *evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  auto runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
  std::ofstream& csvFile = runAction->GetCsvFile();

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  fEdep += edep;
  fEventAction->AddEdep(edep);

  const G4Track *track = step->GetTrack();

  G4TouchableHandle handle = step->GetPreStepPoint()->GetTouchableHandle();
  G4String volumeName = handle->GetVolume()->GetName();

  //if(volumeName == "Envelope")  //when we want to register particles only in particular volume (i.e. envelope) uncomment if
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

      const G4VProcess *proc = step->GetPostStepPoint()->GetProcessDefinedStep();

      int Process;

      if(proc)
        {
          Process = proc->GetProcessType()*1000 + proc->GetProcessSubType();
        }
      else Process = 900; // not sure why, can be zero

      fEventAction->AddStepData(EventID, TrackID, StepNum, ParentID, DaughterPDG, KinEn, DirX, DirY, DirZ, MomX, MomY, MomZ, MomTot, Speed, Volume, Process);

      if(csvFile.is_open())
        {
          csvFile << EventID << "," << TrackID << "," << StepNum << "," << ParentID << "," << DaughterPDG << "," << KinEn / MeV << "," << DirX / cm << "," << DirY / cm << "," << DirZ / cm << "," << MomX / (MeV / c_light) << "," << MomY / (MeV / c_light) << "," << MomZ / (MeV / c_light) << "," << MomTot / (MeV / c_light) << "," << Speed / (cm / s) << "," << fEdep / MeV << "," << Volume << "," << Process <<"\n";
        }
    //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
