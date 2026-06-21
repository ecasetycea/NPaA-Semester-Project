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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4Types.hh>

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  /*===========*\
  |    WORLD    |
  \*===========*/
  // Materials
  G4Material* world_mat   = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* silicon_mat = nist->FindOrBuildMaterial("G4_Si");
  
  // Volumes
  G4Box* solidWorld = new G4Box(
    "World",
    20*cm,
    20*cm,
    20*cm
  );
  G4LogicalVolume* logicalWorld = new G4LogicalVolume(
    solidWorld,
    world_mat,
    "World"
  );
  G4PVPlacement* physicalWorld = new G4PVPlacement(
    nullptr,                  // No rotation
    G4ThreeVector(),         // (0,0,0)
    logicalWorld,
    "World",
    nullptr,        // no mother volume for world
    false,
    0,
    checkOverlaps
  );

  /*==========*\
  |    SLAB    |
  \*==========*/
  // Sizes
  G4double halfThickness = 50. * um; // TODO: be modifiable during runtime
  G4double halfSizeXY    = 5. * cm;   // large enough so ray doesn't go outside

  // Volumes
  G4Box* solidSlab = new G4Box(
    "Slab",
    halfSizeXY,
    halfSizeXY,
    halfThickness
  );
  G4LogicalVolume* logicalSlab = new G4LogicalVolume(
    solidSlab,
    silicon_mat,
    "Silicon Slab"
  );
  new G4PVPlacement(
    nullptr,                    // no rotation
    G4ThreeVector(),           // (0,0,0)
    logicalSlab,
    "Silicon Slab",
    logicalWorld,     // place inside the world
    false,
    0, // no copies
    checkOverlaps
  );



  // Set Shape2 as scoring volume
  fScoringVolume = logicalSlab;



  // always return the physical World
  return physicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
