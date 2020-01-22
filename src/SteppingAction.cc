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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "SteppingAction.hh"
#include "RunAction.hh"
#include "Diagnostics.hh"

#include "G4SteppingManager.hh" // includes all the needed classes for SteppingAction

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Save pointer to the current Diagnostics instance.

*/
SteppingAction::SteppingAction(Diagnostics* diagnostics)
: G4UserSteppingAction(),
  fDiagnostics(diagnostics)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Do nothing.

*/
SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Call the Diagnostics FillDiagXXX methods if they are activated.

This virtual method is called at each Step ends.
*/
void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // Get step points
  // G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

  // Retrieve particle type
  G4Track* aTrack = aStep->GetTrack();
  const G4ParticleDefinition* particle = aTrack->GetDynamicParticle()->GetDefinition();

  // Fill diagnostics if they are activated
  fDiagnostics->FillDiagSurfacePhaseSpace(particle, preStepPoint);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
