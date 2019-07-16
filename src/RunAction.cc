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

#include "G4SystemOfUnits.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4GenericMessenger.hh"
#include "G4Run.hh"

#include "Diagnostics.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Create analysis manager, Ntuples and set UI commands.

*/
RunAction::RunAction()
: G4UserRunAction(),
  fInFileName(""),
  fDiagnostics(nullptr),
  fInMessenger(nullptr)
{
  fDiagnostics = new Diagnostics();

  // set UI commands
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete analysis manager.

*/
RunAction::~RunAction()
{
  delete fDiagnostics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Read input file & open output file.

This user code is executed at the beginning of each run
*/
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  // read input file
  if (!isMaster){
    G4int NbOfEvents = aRun->GetNumberOfEventToBeProcessed();
    ReadInput(NbOfEvents);
  }

  fDiagnostics->InitializeAllDiags();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Write and close output file

This user code is executed at the end of each run
*/
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save Ntuples
  fDiagnostics->FinishAllDiags();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief


*/
void RunAction::ReadInput(G4int NbOfEvents)
{
  // Clear fW,fX,... before import
  fW.clear()    ;
  fX.clear()    ; fY.clear()  ; fZ.clear();
  fPx.clear()   ; fPy.clear() ; fPz.clear();
  fT.clear()    ;
  // Define streams
  std::ifstream input;
  std::string str;
  input.open(fInFileName);

  // Declare temporary variables
  G4double w,x,y,z,px,py,pz,t;

  // Read file
  while (!input.eof())
  {
    // Get lines, ignore comments and empty lines
    std::getline(input,str);
    if(str[0]=='#' or str=="") continue;
    std::stringstream ss(str);
    // Save input data
    ss >> w >> x >> y >> z >> px >> py >> pz >> t;
    fW.push_back(w)   ;
    fX.push_back(x)   ; fY.push_back(y)  ; fZ.push_back(z);
    fPx.push_back(px) ; fPy.push_back(py); fPz.push_back(pz);
    fT.push_back(t)   ;
  }
  input.close();

  // Normalize weights
  G4double normW = (G4double)NbOfEvents/(G4double)fW.size();
  for (unsigned int i=0; i<fW.size(); i++)
  {
    fW[i] = fW[i]/normW;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Define UI commands.

The input file name can be changed by using
/input/setFileName fileName

The output file name can be changed by using
/output/setFileName fileName

*/
void RunAction::SetCommands()
{
  // get UI messengers
  fInMessenger = new G4GenericMessenger(this,"/input/","Manage simulation input");

  // define commands
  G4GenericMessenger::Command& setInFileNameCmd
    = fInMessenger->DeclareProperty("setFileName",
                                fInFileName,
                                "Change input file name");

  // set commands properties
  setInFileNameCmd.SetStates(G4State_Idle);
}
