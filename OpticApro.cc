//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  G4long myseed = 345354;
  for(G4int i = 1; i < argc; i = i + 2)
  {
    if(G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if(G4String(argv[i]) == "-u")
      session = argv[i + 1];
    else if(G4String(argv[i]) == "-r")
      myseed = atoi(argv[i + 1]);
#ifdef G4MULTITHREADED
    else if(G4String(argv[i]) == "-t")
    {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
  }

  // Optional: choose a different Random engine...
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  G4long seed = time(NULL);
  G4Random::setTheSeed(seed);

  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if(macro.size() == 0)
  {
    ui = new G4UIExecutive(argc, argv);
  }

  // Construct the default run manager
  auto runManager = G4RunManagerFactory::CreateRunManager();
#ifdef G4MULTITHREADED
  if(nThreads > 0)
    runManager->SetNumberOfThreads(nThreads);
#endif

  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  // Set mandatory initialization classes

  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  // Action Initialization
  runManager->SetUserInitialization(new ActionInitialization());

  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(macro.size())
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macro);
  }
  else  // Define UI session for interactive mode
  {
    UImanager->ApplyCommand("/control/execute vis.mac");
    if(ui->IsGUI())
      UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
