// DetectorConstruction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  //if cube geometry is needed, uncomment section cuboid geo marked with *
  //if it is sphere geometry that is needed, uncomment section spherical geo marked with #

  //****************************************************************************
  //cube geo
  //two versions:
  //1) CUBE_GMA -> the bigger block filled with air is bigger with overall dimensions 10 cm x 15 cm x 10 cm
  //2) CUBE_GLA -> the bigger block filled with air is much smaller, overall dimensions are 2.02 cm x 3.02 cm x 2.02 cm
  //uncomment each section as needed
  ///*
  //CUBE_GMA
  fAirBlock_World_x = fAirBlock_World_z = 5.0 * cm;
  fAirBlock_World_y = 7.5 * cm;
  fWaterEye_Envelope_x = fWaterEye_Envelope_z = 1.0 * cm;
  fWaterEye_Envelope_y = 1.5 * cm;
  //*/

  /*
  //CUBE_GLA
  fAirBlock_World_x = fAirBlock_World_z = 1.01 * cm;
  fAirBlock_World_y = 1.51 * cm;
  fWaterEye_Envelope_x = fWaterEye_Envelope_z = 1.0 * cm;
  fWaterEye_Envelope_y = 1.5 * cm;
  */
  //****************************************************************************

  //############################################################################
  //sphere geo
  //two versions:
  //1) SPHERE_G-2.0 -> outer air block has dimensions: 10 x 15 x 10 cm, the inner water sphere has diameter 2.0 cm
  //2) SPHERE_G-2.5 -> outer air block has dimensions: 10 x 15 x 10 cm, the inner water sphere has diameter 2.5 cm
  //uncomment each section as needed
  /*
  //SPHERE_G-2.0
  fAirBlock_World_x = fAirBlock_World_z = 5.0 * cm;
  fAirBlock_World_y = 7.5 * cm;
  fWaterEye_Envelope_x = 1.0 * cm;
  */

  /*
  //SPHERE_G-2.5
  fAirBlock_World_x = fAirBlock_World_z = 5.0 * cm;
  fAirBlock_World_y = 7.5 * cm;
  fWaterEye_Envelope_x = 1.25 * cm;
  */
  //############################################################################
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

  // Air
  //
  G4Element* N = new G4Element("Nitrogen", "N", z = 7, a = 14.01 * g / mole);
  G4Element* O = new G4Element("Oxygen", "O", z = 8, a = 16.00 * g / mole);

  G4Material* air =
    new G4Material("Air", density = 1.29 * mg / cm3, nelements = 2);
  air->AddElement(N, 70. * perCent);
  air->AddElement(O, 30. * perCent);

  // Water
  //
  G4Element* H = new G4Element("Hydrogen", "H", z = 1, a = 1.01 * g / mole);

  G4Material* water =
    new G4Material("Water", density = 1.0 * g / cm3, nelements = 2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  // ------------ Generate & Add Material Properties Table ------------

  //wvl range 380 - 700 nm
  std::vector<G4double> photonEnergy = {
    1.7710 * eV, 1.7967 * eV, 1.8230 * eV, 1.8500 * eV, 1.8780 * eV, 1.9070 * eV, 
    1.9370 * eV, 1.9680 * eV, 1.9995 * eV, 2.0320 * eV, 2.0660 * eV, 2.1012 * eV, 
    2.1370 * eV, 2.1750 * eV, 2.2140 * eV, 2.2540 * eV, 2.2957 * eV, 2.3391 * eV, 
    2.3840 * eV, 2.4308 * eV, 2.4790 * eV, 2.5300 * eV, 2.5827 * eV, 2.6380 * eV, 
    2.6950 * eV, 2.7550 * eV, 2.8180 * eV, 2.8830 * eV, 2.9520 * eV, 3.0240 * eV, 
    3.0993 * eV, 3.1790 * eV, 3.2620 * eV
  };

  // Water

  //wvl range 380 - 700  nm
  std::vector<G4double> refractiveIndex1 = {
    1.3305, 1.3307, 1.331, 1.3312, 1.3314, 1.3317, 1.3319, 1.3322, 1.3325,
    1.3327, 1.333, 1.3333, 1.3336, 1.334, 1.3343, 1.3347, 1.3351, 1.3355, 
    1.3359, 1.3363, 1.3368, 1.3373, 1.3378, 1.3384, 1.3390, 1.3396, 1.3403,
    1.3410, 1.3418, 1.3426, 1.3436, 1.3445, 1.3456
  };

  //wvl range 380 - 700  nm
  std::vector<G4double> absorption = {
    1.603 * m, 1.937 * m, 2.151 * m, 2.278 * m, 2.439 * m, 2.941 * m, 3.218 * m,
    3.429 * m, 3.6298 * m, 3.782 * m, 4.496 * m, 7.402 * m, 11.161 * m, 14.388 * m,
    16.155 * m, 17.699 * m, 21.097 * m, 23.041 * m, 24.4498 * m, 30.769 * m, 49.0196 * m,
    66.667 * m, 78.740 * m, 94.3396 * m, 102.145 * m, 108.4599 * m, 157.480 * m,
    220.02 * m, 220.26 * m, 211.416 * m, 150.8296 * m, 117.509 * m, 87.951 * m
  };

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  G4int numEntries = 1;
  myMPT1->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0], numEntries);

  for (size_t i = 1; i < photonEnergy.size(); ++i) {
    myMPT1->AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
  }

  myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption, false, true);

  std::vector<G4double> energy_water = {
    1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV,
    1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
    1.7971 * eV,  1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV,
    1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
    2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV,
    2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
    2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV,
    2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
    3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV,
    3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
    4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV,
    5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV
  };

  // Rayleigh scattering length calculated by G4OpRayleigh

  // assume 100 times larger than the rayleigh scattering for now.
  std::vector<G4double> mie_water = {
    167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m,
    128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
    97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m,
    71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
    51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m,
    36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
    24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m,
    16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
    9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m,
    5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
    3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m,
    1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m
  };

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3] = { 0.99, 0.99, 0.8 };

  myMPT1->AddProperty("MIEHG", energy_water, mie_water, false, true);
  myMPT1->AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable:" << G4endl;
  myMPT1->DumpTable();

  water->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator
  water->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  // Air
  //
  //wvl range 380 - 700 nm
  std::vector<G4double> refractiveIndex2 = {
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
  }; 

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2);

  G4cout << "Air G4MaterialPropertiesTable:" << G4endl;
  myMPT2->DumpTable();

  air->SetMaterialPropertiesTable(myMPT2);

  // ------------- Volumes --------------

  //****************************************************************************
  //cube geo
  ///*
  // The Air block (World Volume)
  //
  G4Box* AirBlock_World_box = new G4Box("World", fAirBlock_World_x, fAirBlock_World_y, fAirBlock_World_z);

  G4LogicalVolume* AirBlock_World_log =
    new G4LogicalVolume(AirBlock_World_box, air, "World", 0, 0, 0);

  G4VPhysicalVolume* AirBlock_World_phys =
    new G4PVPlacement(0, G4ThreeVector(), AirBlock_World_log, "World", 0, false, 0);

  // The Water Eye (Envelope) block
  G4Box* WaterEye_Envelope_box = new G4Box("Envelope", fWaterEye_Envelope_x, fWaterEye_Envelope_y, fWaterEye_Envelope_z);

  G4LogicalVolume* WaterEye_Envelope_log =
  new G4LogicalVolume(WaterEye_Envelope_box, water, "Envelope", 0, 0, 0);

  G4VPhysicalVolume* WaterEye_Envelope_phys = new G4PVPlacement(
  0, G4ThreeVector(), WaterEye_Envelope_log, "Envelope", AirBlock_World_log, false, 0);
  //*/
  //****************************************************************************

  //############################################################################
  //sphere geo
  /*
  // The Air block (World Volume)
  //
  G4Box* AirBlock_World_box = new G4Box("World", fAirBlock_World_x, fAirBlock_World_y, fAirBlock_World_z);

  G4LogicalVolume* AirBlock_World_log =
    new G4LogicalVolume(AirBlock_World_box, air, "World", 0, 0, 0);

  G4VPhysicalVolume* AirBlock_World_phys =
    new G4PVPlacement(0, G4ThreeVector(), AirBlock_World_log, "World", 0, false, 0);

  // The Water Eye (Envelope) spherical
  G4Sphere* WaterEye_Envelope_sphere = new G4Sphere("Envelope", 0.0, fWaterEye_Envelope_x, 0.0, 2.0 * CLHEP::pi, 0.0, CLHEP::pi);

  G4LogicalVolume* WaterEye_Envelope_log =  new G4LogicalVolume(WaterEye_Envelope_sphere, water, "Envelope", 0, 0, 0);

  G4VPhysicalVolume* WaterEye_Envelope_phys = new G4PVPlacement(0, G4ThreeVector(), WaterEye_Envelope_log, "Envelope", AirBlock_World_log, false, 0);
  */
  //############################################################################

  // ------------- Surfaces --------------

  // Water Eye
  //
  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_LUTDAVIS);
  opWaterSurface->SetFinish(Rough_LUT);
  opWaterSurface->SetModel(DAVIS);

  G4LogicalBorderSurface* waterSurface = new G4LogicalBorderSurface(
    "WaterSurface", WaterEye_Envelope_phys, AirBlock_World_phys, opWaterSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(
    waterSurface->GetSurface(WaterEye_Envelope_phys, AirBlock_World_phys)
      ->GetSurfaceProperty());
  if(opticalSurface)
    opticalSurface->DumpInfo();

  // Generate & Add Material Properties Table attached to the optical surfaces
  //
  std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };

  return AirBlock_World_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
