// EventAction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4GenericAnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
  : G4UserEventAction()
{
  fRayleigh   = 0;
  fAbsorption = 0;
  fMie        = 0;
  fBoundary   = 0;
  fEdep = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fRayleigh   = 0;
  fAbsorption = 0;
  fMie        = 0;
  fBoundary   = 0;
  fEdep = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  Run* run = static_cast<Run*>(
    G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  run->AddRayleigh(fRayleigh);
  run->AddAbsorption(fAbsorption);
  run->AddMie(fMie);
  run->AddBoundary(fBoundary);

  event->Print();


  // Fill histograms & ntuple
  auto analysisManager = G4AnalysisManager::Instance();
  //
  for(size_t i=0; i<trkID.size(); ++i)
  {
      analysisManager->FillNtupleIColumn(0, evtID[i]);   //column 0, EventID
      analysisManager->FillNtupleIColumn(1, trkID[i]);   //column 1, trackID
      analysisManager->FillNtupleIColumn(2, stp[i]);      //column 2, step number
      analysisManager->FillNtupleIColumn(3, pID[i]);      //column 3, parentID
      analysisManager->FillNtupleIColumn(4, dPDG[i]);      //column 4, daughter PDG
      analysisManager->FillNtupleDColumn(5, Ekin[i] / MeV);        //column 5, kin. energy [MeV]
      analysisManager->FillNtupleDColumn(6, x[i] / cm);        //column 6, direction x [cm] (origin: center of the world volume)
      analysisManager->FillNtupleDColumn(7, y[i] / cm);        //column 7, direction y [cm] (origin: center of the world volume)
      analysisManager->FillNtupleDColumn(8, z[i] / cm);        //column 8, direction z [cm] (origin: center of the world volume)
      analysisManager->FillNtupleDColumn(9, px[i] / (MeV / c_light));       //column 9, momentum x [MeV/c]
      analysisManager->FillNtupleDColumn(10, py[i] / (MeV / c_light));         //column 10, momentum y [MeV/c]
      analysisManager->FillNtupleDColumn(11, pz[i] / (MeV / c_light));        //column 11, momentum z [MeV/c]
      analysisManager->FillNtupleDColumn(12, pTot[i] / (MeV / c_light));        //column 12, momentum Total [MeV/c]
      analysisManager->FillNtupleDColumn(13, v[i] / (cm / s));       //column 13, velocity [cm/s]
      analysisManager->FillNtupleDColumn(14, fEdep / MeV);        //column 14, deposited energy
      analysisManager->FillNtupleDColumn(15, vol[i]);        //column 15, volume (if World vol = 1, if envelope vol = 2)
      analysisManager->FillNtupleDColumn(16, prc[i]);        //column 16, process type
      analysisManager->AddNtupleRow();
      // Get analysis manager
  }

  //clear vectors for the next event
  evtID.clear();  //EventID
  trkID.clear();  //trackID
  stp.clear();  //step number
  pID.clear();  //parentID
  dPDG.clear(); //daughter PDG
  Ekin.clear(); //kin. energy [MeV]
  x.clear(); //direction x [cm] (origin: center of the world volume)
  y.clear(); //direction y [cm] (origin: center of the world volume)
  z.clear(); //direction z [cm] (origin: center of the world volume)
  px.clear();  //momentum x [MeV/c]
  py.clear();  //momentum y [MeV/c]
  pz.clear();  //momentum z [MeV/c]
  pTot.clear();  //momentum Total [MeV/c]
  v.clear(); //velocity [cm/s]
  vol.clear(); //volume
  prc.clear(); //process type
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddStepData(int EventID, int TrackID, int StepNum, int ParentID, int DaughterPDG, double KinEn, double DirX, double DirY, double DirZ, double MomX, double MomY, double MomZ, double MomTot, double Speed, double Volume, int Process)
{
    evtID.push_back(EventID);  //EventID
    trkID.push_back(TrackID);  //trackID
    stp.push_back(StepNum);  //step number
    pID.push_back(ParentID);  //parentID
    dPDG.push_back(DaughterPDG); //daughter PDG
    Ekin.push_back(KinEn); //kin. energy [MeV]
    x.push_back(DirX); //direction x [cm] (origin: center of the world volume)
    y.push_back(DirY); //direction y [cm] (origin: center of the world volume)
    z.push_back(DirZ); //direction z [cm] (origin: center of the world volume)
    px.push_back(MomX);  //momentum x [MeV/c]
    py.push_back(MomY);  //momentum y [MeV/c]
    pz.push_back(MomZ);  //momentum z [MeV/c]
    pTot.push_back(MomTot);  //momentum Total [MeV/c]
    v.push_back(Speed); //velocity [cm/s]
    vol.push_back(Volume); //volume (if World vol = 1, if envelope vol = 2)
    prc.push_back(Process); //process (legend v README-process)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
