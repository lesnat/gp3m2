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
/// \file RunAction.hh
/// \brief Definition of the RunAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "Analysis.hh"
#include "G4ThreeVector.hh"

class G4Run;
class G4ParticleDefinition;
class Units;
class InputReader;
class Diagnostics;
#include "G4GenericMessenger.hh"

/**
\brief Deal with input file reading and diagnostic creation.

*/
class RunAction : public G4UserRunAction
{
  public:
    RunAction(Units* units);
    ~RunAction();

    // base class methods
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void   EndOfRunAction(const G4Run*);

    // get/set methods
    InputReader* GetInputReader() {return fInputReader;};
    Diagnostics* GetDiagnostics() {return fDiagnostics;};

  private:
    // Geant4 pointers

    // User pointers
    Units* fUnits; /**< \brief Pointer to the Units instance.*/
    InputReader* fInputReader; /**< \brief Pointer to the InputReader instance.*/
    Diagnostics* fDiagnostics; /**< \brief Pointer to the Diagnostics instance.*/

    // User variables

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
