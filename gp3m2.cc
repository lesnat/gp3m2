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
/// \brief main file allowing to launch the application into different modes
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "G4EmPenelopePhysics.hh"
#include "ActionInitialization.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // construct the default run manager
  #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
  #else
    G4RunManager* runManager = new G4RunManager;
  #endif

  // set mandatory initialization classes
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new G4EmPenelopePhysics);

  // set user action classes
  runManager->SetUserInitialization(new ActionInitialization);

  // initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // launch the app with the choosen mode
  if (argv[1]=="-i" or argc==1)         // interactive mode
  {
    // initialize interactive session
    UImanager->ApplyCommand("/control/execute init.mac");

    // start interactive session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    delete ui;
  }
  else if (argv[1]=="-v")               // visualization mode
  {
    // initialize interactive session and visualization
    UImanager->ApplyCommand("/control/execute init.mac");
    UImanager->ApplyCommand("/control/execute init_vis.mac");

    // start interactive session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    delete ui;
  }
  else if (argv[1]=="-m" and argc==3)   // launch a macro
  {
    UImanager->ApplyCommand("/control/execute "+argv[2]);
  }
  else
  {
    // print usage
    G4cerr << " Usage: " << G4endl;
    G4cerr << " gp3m2 -i        :"
           << " launch the application in interactive mode (default)" << G4endl;
    G4cerr << " gp3m2 -v        :"
           << " launch the application in visualization mode" << G4endl;
    G4cerr << " gp3m2 -m macro  :"
           << " launch a the macro file `macro`" << G4endl;
    G4cerr << G4endl;

    // return error code
    return 1;
  }

  // job termination
  delete runManager;

  // return 0 if no errors
  return 0;
}
