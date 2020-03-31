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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "G4RunManager.hh" // includes all the needed classes for RunAction
#include "G4Run.hh"

#include "Units.hh"
#include "InputReader.hh"
#include "Diagnostics.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief .

*/
RunAction::RunAction(Units* units, InputReader* inputReader, Diagnostics* diagnostics)
: G4UserRunAction(),
  fUnits(units),
  fInputReader(inputReader),
  fDiagnostics(diagnostics)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete.

*/
RunAction::~RunAction()
{
  delete fInputReader;
  delete fDiagnostics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Read input file & initialize diagnostics.

This user code is executed at the beginning of each run
*/
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  // read input file
  fInputReader->ReadInputFile();
  fInputReader->NormalizeMacroParticlesWeights(aRun->GetNumberOfEventToBeProcessed());

  // Initialize diagnostics
  fDiagnostics->InitializeAllDiags();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Write and close output files.

This user code is executed at the end of each run
*/
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save diagnostics
  fDiagnostics->FinishAllDiags();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
