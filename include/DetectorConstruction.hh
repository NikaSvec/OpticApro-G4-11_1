// DetectorConstruction header
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct() override;

 private:
  G4double fAirBlock_World_x;
  G4double fAirBlock_World_y;
  G4double fAirBlock_World_z;

  G4double fWaterEye_Envelope_x;
  G4double fWaterEye_Envelope_y;
  G4double fWaterEye_Envelope_z;
};

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
