//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the B1::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <G4Exception.hh>
#include <G4ThreeVector.hh>

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // default particle kinematic - will be overriden during execution
  G4ParticleDefinition* particle =
    G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleEnergy(6. * MeV);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // this function is called at the begining of each event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  // Set particle position to before the slab
  G4double worldSizeZ = 0.;
  
  if(!worldBox) {
    G4LogicalVolume* world = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    worldBox = dynamic_cast<G4Box*>(world->GetSolid());
  }

  if(worldBox) {
    worldSizeZ = worldBox->GetZHalfLength() * 2.;
  }
  else {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found.\n";
    msg << "Perhaps you gave a different name to the world in DetectorConstruction.cc.\n";
    msg << "The gun will be placed at the center.";
    G4Exception(
      "PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002",
      JustWarning,
      msg
    );
  }

  G4double x0 = 0.;
  G4double y0 = 0.;
  G4double z0 = -0.45 * worldSizeZ; // -0.45 to start slightly inside, 0.5 is on the border

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->GeneratePrimaryVertex(event);
}


void PrimaryGeneratorAction::SetParticle(const G4String& particleName) {
  G4ParticleDefinition* particle =
    G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  
  if(particle) {    // only apply if particle found
    fParticleGun->SetParticleDefinition(particle);
  } else {
    G4cerr << "Unknown particle: " << particleName << G4endl;
  }
}


void PrimaryGeneratorAction::SetEnergy(G4double particleEnergy) {
  fParticleGun->SetParticleEnergy(particleEnergy);
}


}  // namespace B1
