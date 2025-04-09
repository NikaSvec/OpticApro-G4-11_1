// PrimaryGeneratorAction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class PrimaryGeneratorMessenger;
class G4Box;
class G4Sphere;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
 public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*) override;

  void SetOptPhotonPolar();
  void SetOptPhotonPolar(G4double);

  G4ParticleGun* GetParticleGun() { return fParticleGun; }

 private:
  G4ParticleGun* fParticleGun;
  PrimaryGeneratorMessenger* fGunMessenger;
  G4Box* fEnvelopeBox;
  G4Sphere* fEnvelopeSphere;
  G4Box* fWorldBox;
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
