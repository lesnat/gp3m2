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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Set default cut values, verbosity and instanciate desired pre-packages PhysicsList.

*/
PhysicsList::PhysicsList()
: G4VModularPhysicsList(),
  fMessenger(0)
{
  // set default cut value
  SetDefaultCutValue(1.0*um);
  SetVerboseLevel(1);
  // EM physics
  SetPhysicsList("penelope");
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete pre-packaged PhysicsList instance

*/
PhysicsList::~PhysicsList()
{
  delete fPhysicsList;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Construct the particles to consider in the simulation.

The list of particles is taken from choosen pre-packaged PhysicsList.
*/
void PhysicsList::ConstructParticle()
{
  fPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Construct the physics processes to use in the simulation.

The list of procresses is taken from choosen pre-packaged PhysicsList.
*/
void PhysicsList::ConstructProcess()
{
  // Transportation
  AddTransportation();

  // Electromagnetic physics list
  fPhysicsList->ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Call base class method to set cuts which default value can be
       modified via /run/setCut/_ commands

?
*/
void PhysicsList::SetCuts()
{
 G4VUserPhysicsList::SetCuts();

 DumpCutValuesTable();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::SetPhysicsList(G4String name)
{
  if (name == "simple")
  {
    //fPhysicsList = new PhysicsListSimple();
  }
  else if (name == "penelope")
  {
    fPhysicsList = new G4EmPenelopePhysics();
  }
  else if (name == "standard")
  {
    fPhysicsList = new G4EmStandardPhysics();
  }
  else
  {
    G4cerr << "Unknown physics list name : " << name << G4endl;
  }

  G4cout << "Physics list has been modified to : " << name << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Set commands to be interpreted with the UI

The setPhysicsList function can be called in UI in the following way :

/physics/setPhysicsList str
*/
void PhysicsList::SetCommands()
{
  // get UI messengers
  fMessenger = new G4GenericMessenger(this,"/physics/","Change physics list");

  // define commands
  G4GenericMessenger::Command& setPhysicsListCmd
    = fMessenger->DeclareMethod("setPhysicsList",
                                &PhysicsList::SetPhysicsList,
                                "Change physics list");

  // set commands properties
  setPhysicsListCmd.SetStates(G4State_Idle);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
