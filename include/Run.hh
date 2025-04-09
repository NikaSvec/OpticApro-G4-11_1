// Run header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"

class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run : public G4Run
{
 public:
  Run();
  ~Run();

  void SetPrimary(G4ParticleDefinition* particle, G4double energy);

  void AddCerenkov(G4double n)
  {
    fCerenkovCounter += n;
    fCerenkov2 += n * n;
  };
  void AddRayleigh(G4double n)
  {
    fRayleighCounter += n;
    fRayleigh2 += n * n;
  };
  void AddAbsorption(G4double n)
  {
    fAbsorptionCounter += n;
    fAbsorption2 += n * n;
  };
  void AddMie(G4double n)
  {
    fMieCounter += n;
    fMie2 += n * n;
  };
  void AddBoundary(G4double n)
  {
    fBoundaryCounter += n;
    fBoundary2 += n * n;
  };

  void Merge(const G4Run*) override;
  void EndOfRun();

 private:
  G4double fCerenkovCounter;
  G4double fCerenkov2;
  G4double fRayleighCounter;
  G4double fRayleigh2;
  G4double fAbsorptionCounter;
  G4double fAbsorption2;
  G4double fMieCounter;
  G4double fMie2;
  G4double fBoundaryCounter;
  G4double fBoundary2;

  G4ParticleDefinition* fParticle;
  G4double fEnergy;
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
