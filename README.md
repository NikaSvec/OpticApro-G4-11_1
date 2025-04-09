# OpticApro
## Geant4 ver. 10.7

> [!NOTE]
> This version of OpticApro uses Geant4 11.1. If you are using Geant4 10.  
> please look for OpticApro version OpticApro-G4-10\_7

***

## Licence

This project is licensed under the terms of the BSD 2-Clause "Simplified" License.  
For more detailes see file LICENSE.

***

## Summary

This model was created to study the phenomenon of light flashes. The simulation represents  
interactions of different particles found in cosmic rays with human eye. Simulation contains  
four different models. Specifically:

1. CUBE\_GMA or Cube geometry\_Geant4 More Air
2. CUBE\_GLA or Cube geometry\_Geant4 Low Air
3. SPHERE\_G-2.0 or Sphere geometry\_Geant4 - 2.0 diameter
4. SPHERE\_G-2.5 or Sphere geometry\_Geant4 - 2.5 diameter.

## List of headers & sources

### Headers:

- ActionInitialization.hh 
- DetectorConstruction.hh 
- EventAction.hh 
- PrimaryGeneratorAction.hh
- PrimaryGeneratorMessenger.hh
- RunAction.hh
- Run.hh
- StackingAction.hh
- SteppingAction.hh
- TrackingAction.hh

### Sources:

- OpticApro.cc
- ActionInitialization.cc
- DetectorConstruction.cc
- EventAction.cc
- PrimaryGeneratorAction.cc
- PrimaryGeneratorMessenger.cc
- RunAction.cc
- Run.cc
- StackingAction.cc
- SteppingAction.cc
- TrackingAction.cc

## Description

### OpticApro.cc

- contains main() 
  - defining Random Number generator & seed, runManager
  - initialization of classes: DetectorConstruction, Physics list, G4OpticalPhysics,  
    ActionInitialization, Vizualisation manager, UI manager
  - Physics list used: FTFP\_BERT
  - G4Optical Physics includes definition of:
    - particles: 
      - G4OpticalPhoton
    - processes: 
      - G4Cerenkov  
      - G4OpAbsorption  
      - G4OpMieHG  
      - G4OpRayleigh  
      - G4OpBoundaryProcess

### ActionInitialization.hh & .cc

- initialization of all other classes

### DetectorConstruction.hh & .cc

- defining material: Air and water
- definig geometry of the model (4 different models)
- parts of the code divided dependent on which model is chosen.

- cube geometry: 
  - if cube geometry is needed, uncomment the sections of code within two `//*********`
  - inside the section are two models:
    - CUBE\_GMA: -marked with `//CUBE_GMA`
      - two concentric blocks
      - air block: 10 x 15 x 10 cm
      - water block: 2 x 3 x 2 cm
    - CUBE\_GLA: -marked with `//CUBE_GLA`
      - two concentric blocks
      - air block: 2.02 x 3.02 x 2.02 cm
      - water block: 2 x 3 x 2 cm

- sphere geometry: 
  - if sphere geometry is needed, uncomment the sections of code within two `//##############`
  - inside the section are two models:
    - SPHERE\_G-2.0: -marked with `SPHERE_G-2.0`
      - a block with a sphere in the center
      - air block: 10 x 15 x 10 cm
      - water sphere: diameter d = 2.0 cm
    - SPHERE\_G-2.5: -marked with `SPHERE_G-2.5`
      - a block with a sphere in the center
      - air block: 10 x 15 x 10 cm
      - water sphere: diameter d = 2.5 cm

- includes definitions for (difference from G4 10.7 in defining  
some of the variables):
  - refractive indices for water and air
  - photon energies for wavelength range 380 - 700 nm
  - absorption of water
  - Mie scattering
  - Rayleigh scattering
  - G4MaterialPropertiesTable

### PrimaryGeneratorAction.hh & .cc

- contains definition of:
  - primary particles injected into the model (can be changed in startup macro)
  - position of the entrance of primary particle
  - energy of primary particle (can be changed in startup macro)

- devided in parts because of the 4 models:
  - cube geometry: - marked with two `//*************`
    - section for CUBE_GMA, marked with `//CUBE_GMA`
    - section for CUBE_GLA, marked with `//CUBE_GMA`
  - sphere geometry: - marked with two `//############`

### SteppingAction.hh & .cc and TrackingAction.hh & .cc

- used to record:
  - EventID
  - TrackID
  - StepNum
  - ParentID
  - DaughterPDG
  - KinEn
  - Position X, Y, Z, (name of variable: DirX, DirY, DirZ)
  - Momentum X, Y, Z, Total, (name of variable: MomX, MomY, MomZ, MomTot)
  - Speed
  - Energy deposition (name of variable: fEdep)
  - Volume (notes in which volume particle currently resides)
  - Process type (what process is running, legend in README-process)

- used to define the G4OpAbsorption, G4OpMieHG, G4OpRayleigh, G4OpBoundaryProcess
- SteppingAction records all steps from first to last
- TrackingAction records information of step 0. 

### StackingAction.hh & .cc

- definition of G4OpticalPhoton and Cherenkov process and counter

### Run.hh & .cc

- optical processes counter - counts the number of optical photons created by each optical process

### EventAction.hh & .cc and RunAction.hh & .cc

- used to accumulate the statistics in two types of files:
  - .root file
  - .csv file

- RunAction defines the name of the files through baseFile, which can be defined  
  in startup macro, or directly in the RunAction if OpenGL is used

### Macros:

- simulation includes these macros:
  - gui.mac: graphical user interface configuration
  - vis.mac: visualisation configuration
  - run1.mac: startup macro for primary particle: proton
  - run2.mac: startup macro for primary particles: alpha, anti-muon

### How to start:

- compile and generate an executable (cmake & make)
- to execute in "batch" mode use:

  ```
  $ ./OpticApro -m run1.mac
  ```
  or
  ```
  $ nohup ./OpticApro -m run1.mac > $PATH_where_you_want_dump/run.txt &
  ```

- to execute in "interactive mode":
  - first change the name of the .root and .csv files from baseFile to the option:
    
    ```
    32 analysisManager->SetFileName("output.root");
    .
    .
    .
    97 G4String csvName = "output.csv";
    ```
  
  - than in terminal type:
  
    ```
    $ ./OpticApro
    ```
  
  - to execute macro in OpenGL type: `/control/execute run1.mac`

