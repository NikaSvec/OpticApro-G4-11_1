// RunAction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "fstream"

class PrimaryGeneratorAction;
class Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class RunAction : public G4UserRunAction
{
 public:
  RunAction(PrimaryGeneratorAction* = nullptr);
  ~RunAction();

  G4Run* GenerateRun() override;
  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;
  std::ofstream& GetCsvFile() {return csvFile;}

 private:
  Run* fRun;
  PrimaryGeneratorAction* fPrimary;
  std::ofstream csvFile;
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
