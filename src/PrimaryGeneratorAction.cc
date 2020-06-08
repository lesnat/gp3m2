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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction.hh"
#include "InputReader.hh"

#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"

#include "G4Event.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4ParticleGun.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Retrieve the G4ParticleTable instance.

*/
PrimaryGeneratorAction::PrimaryGeneratorAction(InputReader* inputReader)
: G4VUserPrimaryGeneratorAction(),
  fParticleTable(nullptr),
  fInputReader(inputReader)
{
  // get particle table instance
  fParticleTable = G4ParticleTable::GetParticleTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Do nothing.

*/
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Generate primary particles.

The primary particle is defined with properties of a random input macro-particle.

This virtual function is called at the begining of each event.
*/
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // get particle definition
  G4ParticleDefinition* particleDefinition
    = fParticleTable->FindParticle(fInputReader->GetParticleName());

  // create a primary particle
  G4PrimaryParticle* particle = new G4PrimaryParticle(particleDefinition);

  // pick a random macro-particle
  G4int id = std::floor(G4UniformRand() * fInputReader->GetNumberOfMacroParticles());

  // set macro-particle statistical weight
  G4double w = fInputReader->GetMacroParticleWeight(id);
  particle->SetWeight(w);

  // set macro-particle momentum
  G4ThreeVector p = fInputReader->GetMacroParticleMomentum(id);
  particle->SetMomentum(p[0],p[1],p[2]);

  // get macro-particle position and time
  G4ThreeVector r = fInputReader->GetMacroParticlePosition(id);
  G4double t = fInputReader->GetMacroParticleTime(id);

  // set macro-particle position and time
  G4PrimaryVertex* vertex = new G4PrimaryVertex(r,t);
  vertex->SetPrimary(particle);

  // generate the event
  anEvent->AddPrimaryVertex(vertex);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
