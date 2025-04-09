// PrimaryGeneratorAction source
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction()
  , fParticleGun(nullptr)
{
  G4int n_particle = 1;
  fParticleGun     = new G4ParticleGun(n_particle);

  // create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);

  // default kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="proton");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0 * ns);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleEnergy(100.0 * MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //if cube geometry is needed uncomment section cuboid geo marked with *
  //if sphere geometry is in use, uncomment section spherical geo marked with #
  //****************************************************************************
  //cube geo
  ///*
  G4double envSizeX = 0;
  G4double envSizeY = 0;
  G4double envSizeZ = 0;
  //*/

  ///*
  if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }
  //*/

  ///*
  if(fEnvelopeBox)
  {
    //if geometry with bigger air block is in use, uncomment section CUBE_GMA,
    //if it is geometry with smaller air block use section CUBE_GLA.

    ///*
    //CUBE_GMA
    envSizeX = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeY = fEnvelopeBox->GetYHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*5.;
    //*/

    /*
    //CUBE_GLA
    envSizeX = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeY = fEnvelopeBox->GetYHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetXHalfLength()*2.;
    */
    ///*
  }
  else{
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGenAct::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
  //*/

  //cube geo point-source
  ///*
  G4double size = 0.;
  G4double x0 = size * envSizeX * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeY * (G4UniformRand()-0.5);
  G4double z0 = -1. * envSizeZ;
  //*/
  //****************************************************************************

  //############################################################################
  //sphere geo
  /*
  G4double envSizeX = 0;
  G4double envSizeY = 0;
  G4double envSizeZ = 0;
  */

  /*
  if (!fEnvelopeSphere)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if (envLV) fEnvelopeSphere = dynamic_cast<G4Sphere*>(envLV->GetSolid());
  }
  */

  /*
  if ( fEnvelopeSphere )
  {
    ///*
    //SPHERE_G-2.0/SPHERE_G-2.5
    envSizeX = fEnvelopeSphere->GetOuterRadius()*2.;
    envSizeY = fEnvelopeSphere->GetOuterRadius()*2.;
    envSizeZ = fEnvelopeSphere->GetOuterRadius()*5.;
    //*/
   /*
  }
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGenAct::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
  */

  //sphere geo point-source
  /*
  G4double size = 0.;
  G4double x0 = size * envSizeX * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeY * (G4UniformRand()-0.5);
  G4double z0 = -1. * envSizeZ;
  */
  //############################################################################

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetOptPhotonPolar()
{
  G4double angle = G4UniformRand() * 360.0 * deg;
  SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  if(fParticleGun->GetParticleDefinition()->GetParticleName() !=
     "opticalphoton")
  {
    G4ExceptionDescription ed;
    ed << "Warning: the particleGun is not an opticalphoton";
    G4Exception("PrimaryGeneratorAction::SetOptPhotonPolar()",
                "OpticApro_010", JustWarning, ed);
    return;
  }

  G4ThreeVector normal(1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product * product;

  G4ThreeVector e_perpend(0., 0., 1.);
  if(modul2 > 0.)
    e_perpend = (1. / std::sqrt(modul2)) * product;
  G4ThreeVector e_paralle = e_perpend.cross(kphoton);

  G4ThreeVector polar =
    std::cos(angle) * e_paralle + std::sin(angle) * e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
