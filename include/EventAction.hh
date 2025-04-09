// EventAction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "vector"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
 public:
  EventAction();
  ~EventAction();

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event* event) override;

  void AddRayleigh() { ++fRayleigh; }
  void AddAbsorption() { ++fAbsorption; }
  void AddMie() { ++fMie; }
  void AddBoundary() { ++fBoundary; }
  void AddEdep(G4double edep) {fEdep += edep;}

  void AddStepData(int EventID, int TrackID, int StepNum, int ParentID, int DaughterPDG, double KinEn, double DirX, double DirY, double DirZ, double MomX, double MomY, double MomZ, double MomTot, double Speed, double Volume, int Process);

 private:
  G4int fRayleigh;
  G4int fAbsorption;
  G4int fMie;
  G4int fBoundary;
  G4double fEdep;

  std::vector<int> evtID;  //Event ID (EventID)
  std::vector<int> trkID;  //track ID (TrackID)
  std::vector<int> stp;  //step number (StepNum)
  std::vector<int> pID;  //parentID (ParentID)
  std::vector<int> dPDG; //daughter PDG (DaughterPDG)
  std::vector<double> Ekin; //kin. energy [MeV] (KinEn)
  std::vector<double> x; //direction x [cm] (origin: center of the world volume) (DirX)
  std::vector<double> y; //direction y [cm] (origin: center of the world volume) (DirY)
  std::vector<double> z; //direction z [cm] (origin: center of the world volume) (DirZ)
  std::vector<double> px;  //momentum x [MeV/c] (MomX)
  std::vector<double> py;  //momentum y [MeV/c] (MomY)
  std::vector<double> pz;  //momentum z [MeV/c] (MomZ)
  std::vector<double> pTot;  //momentum Total [MeV/c] (MomTot)
  std::vector<double> v; //velocity [cm/s] (Speed)
  std::vector<double> vol; //volume (if World vol = 1, if envelope vol = 2) (Volume)
  std::vector<int> prc;  //process (vysvetlivka v README-process)

};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
