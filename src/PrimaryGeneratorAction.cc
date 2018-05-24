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
#include "RunAction.hh"

#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"

#include "G4Event.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Instanciate G4ParticleGun and define default primary particles properties

*/
PrimaryGeneratorAction::PrimaryGeneratorAction(RunAction* masterRunAction)
: G4VUserPrimaryGeneratorAction(),
  fParticleDefinition(0),
  fMasterRunAction(masterRunAction)
{

  // define primary particle type
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  fParticleDefinition = particleTable->FindParticle("e-");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete G4ParticleGun instance

*/
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Generate primary particles

This virtual function is called at the begining of each event
*/
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4int id = std::floor(G4UniformRand() * fMasterRunAction->GetLength());

  // statistical weight
  G4double w0 = fMasterRunAction->GetEntry("w",id);

  // position
  G4double x0 = fMasterRunAction->GetEntry("x",id);
  G4double y0 = fMasterRunAction->GetEntry("y",id);
  G4double z0 = fMasterRunAction->GetEntry("z",id);

  // momentum
  G4double px0 = fMasterRunAction->GetEntry("px",id);
  G4double py0 = fMasterRunAction->GetEntry("py",id);
  G4double pz0 = fMasterRunAction->GetEntry("pz",id);

  // initial time
  G4double t0 = fMasterRunAction->GetEntry("t",id);

  G4PrimaryParticle* particle = new G4PrimaryParticle(fParticleDefinition);
  
  particle->SetWeight(w0);
  particle->SetMomentum(px0*MeV,py0*MeV,pz0*MeV);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(x0*um,y0*um,z0*um),t0*1e-3*ps);
  vertex->SetPrimary(particle);

  anEvent->AddPrimaryVertex(vertex);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
