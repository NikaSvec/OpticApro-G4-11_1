// RunAction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Run.hh"

#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Run.hh"
#include "G4GenericAnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "fstream"
#include "string"

using G4AnalysisManager = G4GenericAnalysisManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction* prim)
  : G4UserRunAction()
  , fRun(nullptr)
  , fPrimary(prim)
{
  // Create the generic analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4String baseFile = analysisManager->GetFileName();
  analysisManager->SetVerboseLevel(2);
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName(baseFile + ".root");
  //analysisManager->SetFileName("output.root");  //in case the simulation is run through OpenGL use this instead of baseFile + ".root"
  
  // Book histograms, ntuple

  // Creating ntuple (upravit)
  analysisManager->CreateNtuple("output", "Track");
  analysisManager->CreateNtupleIColumn("EventID");      //column 0
  analysisManager->CreateNtupleIColumn("TrackID");      //column 1
  analysisManager->CreateNtupleIColumn("StepNumber");      //column 2
  analysisManager->CreateNtupleIColumn("ParentID");     //column 3
  analysisManager->CreateNtupleIColumn("DaughterPDG");  //column 4
  analysisManager->CreateNtupleDColumn("Ekin");         //column 5 in MeV
  analysisManager->CreateNtupleDColumn("Direc_x");      //column 6 in cm
  analysisManager->CreateNtupleDColumn("Direc_y");      //column 7 in cm
  analysisManager->CreateNtupleDColumn("Direc_z");      //column 8 in cm
  analysisManager->CreateNtupleDColumn("Momentum_px");  //column 9 in MeV/c
  analysisManager->CreateNtupleDColumn("Momentum_py");  //column 10 in MeV/c
  analysisManager->CreateNtupleDColumn("Momentum_pz");  //column 11 in MeV/c
  analysisManager->CreateNtupleDColumn("Momentum_tot"); //column 12 in MeV/c
  analysisManager->CreateNtupleDColumn("Velocity");      //column 13 in cm/s
  analysisManager->CreateNtupleDColumn("Edep");         //column 14
  analysisManager->CreateNtupleDColumn("Volume");         //column 15
  analysisManager->CreateNtupleIColumn("Process");          //column 16
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  if(csvFile.is_open())
    {
        csvFile.close();
    }
  delete G4AnalysisManager::Instance();
}

G4Run* RunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{

  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  if(fPrimary)
  {
    G4ParticleDefinition* particle =
      fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  //open CSV file
  G4String baseFile = analysisManager->GetFileName();
  G4String csvName = baseFile + ".csv";
  //G4String csvName = "output.csv";   //in case the simulation is run through OpenGL use this instead of baseFile + ".csv"
  csvFile.open(csvName);
  if(csvFile.is_open())
    {
        //write csv header
        csvFile << "EventID, TrackID, StepNum, ParentID, PDG, KinEn, DirX, DirY, DirZ, MomX, MomY, MomZ, MomTot, Speed, Edep, Volume, Process type\n";
    }

  // Open an output file
  // name of the baseFile must be written in a startup macro (e.g. run1.mac)
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  // save ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  //close csv file
  if(csvFile.is_open())
    {
        csvFile.close();
    }

  if(isMaster)
    fRun->EndOfRun();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
