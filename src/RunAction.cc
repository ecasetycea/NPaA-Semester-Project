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
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <string>

namespace B1
{

  G4int RunAction::currentRun = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fEdep);
  accumulableManager->Register(fEdep2);
  accumulableManager->Register(fLength);
  accumulableManager->Register(fLength2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  // get mean values
  G4double totalEdep = fEdep.GetValue();
  G4double totalEdep2 = fEdep2.GetValue();
  G4double totalLength = fLength.GetValue();
  G4double totalLength2 = fLength2.GetValue();

  // calculate mean values
  G4double energy = totalEdep / nofEvents;
  G4double length = totalLength / nofEvents;

  // calculate deviations from mean value
  // this is N*sigma*sigma
  G4double deltaEnergy = totalEdep2 - totalEdep * totalEdep / nofEvents;
  G4double deltaLength = totalLength2 - totalLength * totalLength / nofEvents;

  // get sqrt(N)*sigma (has safeguard for root of negative number)
  deltaEnergy = std::sqrt( std::abs(deltaEnergy) );
  deltaLength = std::sqrt( std::abs(deltaLength) );

  // get standard error of mean sigma/sqrt(N)
  deltaEnergy /= nofEvents;
  deltaLength /= nofEvents;

  // get stopping power + propagation error of deviations above
  // prevent division by 0
  if (length == 0.) {
    //G4cout << "WARNING: LENGTH = " << G4BestUnit(length, "Length") << G4endl;
    // Output a warning and skip the division
    G4cout << "Warning: zero track length in run, skipping dE/dx" << G4endl;

    if(IsMaster()) {
      currentRun++;
      G4cerr << "no track on run number " << currentRun << G4endl;
      
      G4cout << "start_split" << G4endl;
      G4cout << "NaN" << " " << "NaN" << G4endl;
    }
    return;
  }
  G4double DEDX = energy / length;
  G4double deltaDEDX = DEDX * std::sqrt(
    ( deltaEnergy*deltaEnergy / (energy*energy) )
    + ( deltaLength*deltaLength / (length*length) )    
  );




  if(IsMaster()) {
    /*
    G4cout << "start_split" << G4endl;
    G4cout << "--------------------End of Global Run-----------------------" << G4endl;
    G4cout << "The run consists of " + std::to_string(nofEvents) + "events." << G4endl;
    G4cout << "<E-loss> = " << G4BestUnit(energy, "Energy") << G4endl;
    G4cout << "delta<E-loss> = " << G4BestUnit(deltaEnergy, "Energy") << G4endl;
    G4cout << "<x> = " << G4BestUnit(length, "Length") << G4endl;
    G4cout << "delta<x> = " << G4BestUnit(deltaLength, "Length") << G4endl;
    G4cout << "dE/dx = " << G4BestUnit(DEDX, "Energy/Length") << G4endl;
    G4cout << "delta(dE/dx) = " << G4BestUnit(deltaDEDX, "Energy/Length") << G4endl;
    G4cout << "------------------------------------------------------------" << G4endl;

    G4cerr << "Finished run with E = " << G4BestUnit(energy, "Energy") << G4endl;
    */
    currentRun++;
    G4cerr << "finished run number " << currentRun << G4endl;

    G4cout << "start_split" << G4endl;
    G4cout << DEDX * cm / MeV << " " << deltaDEDX * cm / MeV << G4endl;
    G4cout << "DEDX (MeV/cm)" << " " << "deltaDEDX (MeV/cm)" << G4endl;
    G4cout.flush();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep += edep;
  fEdep2 += edep * edep;
  //G4cout << "energy added:" << G4BestUnit(edep, "Energy") <<  G4endl;
}

void RunAction::AddLength(G4double length) {
  fLength += length;
  fLength2 += length * length;
  //G4cout << "length added:" << G4BestUnit(length, "Length") <<  G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
