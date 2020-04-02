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
/// \file gp3m2.cc
/// \brief Main program of gp3m2
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4MTRunManager.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Units.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // construct the default run manager
  G4MTRunManager* runManager = new G4MTRunManager;

  Units* units = new Units();

  // set mandatory initialization classes
  runManager->SetUserInitialization(new DetectorConstruction(units));
  runManager->SetUserInitialization(new PhysicsList);

  // set user action classes
  runManager->SetUserInitialization(new ActionInitialization(units));

  // get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // launch the app with the choosen mode
  G4String mode = argv[1];
  if (mode=="-v" or argc==1)               // visualization mode (default)
  {
    // initialize interactive session and visualization
    G4VisManager* visManager = new G4VisExecutive;
    UImanager->ApplyCommand("/control/execute init_vis.mac");

    // start interactive session
    G4UIExecutive* ui = new G4UIExecutive(1, &argv[0]);
    ui->SessionStart();
    delete ui;
    delete visManager;
  }
  else if (mode=="-m" and argc==3)   // launch a macro
  {
    // launch the macro file
    G4String command = "/control/execute ";
    G4String macro = argv[2];
    UImanager->ApplyCommand(command+macro);
  }
  else
  {
    // print usage
    G4cerr << " Usage: " << G4endl;
    G4cerr << " gp3m2 -i        :"
           << " launch the application in interactive mode (default)" << G4endl;
    G4cerr << " gp3m2 -v        :"
           << " launch the application in visualization mode (default)" << G4endl;
    G4cerr << " gp3m2 -m macro  :"
           << " launch the macro file `macro`" << G4endl;
    G4cerr << G4endl;

    // return error code
    return 1;
  }

  // job termination
  delete runManager;

  // return 0 if no errors
  return 0;
}
