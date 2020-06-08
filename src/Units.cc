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
/// \file Units.cc
/// \brief Implementation of the Units class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Units.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Create analysis manager, Ntuples and set UI commands.

*/
Units::Units()
: fMessenger(nullptr),
  fPositionUnitLabel(""),
  fMomentumUnitLabel(""),
  fTimeUnitLabel("")
{
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete analysis manager.

*/
Units::~Units()
{
  delete fMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Define UI commands.

The input file name can be changed by using

*/
void Units::SetCommands()
{
  // get UI messengers
  fMessenger = new G4GenericMessenger(this,"/units/","Manage simulation units");

  // define commands
  G4GenericMessenger::Command& setPositionUnitCmd
    = fMessenger->DeclareMethod("setPositionUnit",
                                &Units::SetPositionUnit,
                                "Change units of the positions");

  G4GenericMessenger::Command& setMomentumUnitCmd
    = fMessenger->DeclareMethod("setMomentumUnit",
                                &Units::SetMomentumUnit,
                                "Change units of the momentums (in unit/c)");

  G4GenericMessenger::Command& setTimeUnitCmd
    = fMessenger->DeclareMethod("setTimeUnit",
                                &Units::SetTimeUnit,
                                "Change units of the times");

  // set commands properties
  setPositionUnitCmd.SetStates(G4State_PreInit);
  setMomentumUnitCmd.SetStates(G4State_PreInit);
  setTimeUnitCmd.SetStates(G4State_PreInit);
}
