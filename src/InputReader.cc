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
/// \file InputReader.cc
/// \brief Implementation of the InputReader class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "InputReader.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Create analysis manager, Ntuples and set UI commands.

*/
InputReader::InputReader()
: fNumberOfEvents(0),
  fInFileName(""),
  fMessenger(nullptr)
{
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete analysis manager.

*/
InputReader::~InputReader()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief


*/
void InputReader::ReadInputFile()
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief


*/
void InputReader::NormalizeMacroParticlesWeights(G4int NumberOfEventsToBeProcessed)
{
  G4int NumberOfMacroParticles = fW.size();
  G4double normW = (G4double)NumberOfEventsToBeProcessed/(G4double)NumberOfMacroParticles;
  for (unsigned int i=0; i<NumberOfMacroParticles; i++)
  {
    fW[i] = fW[i]/normW;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Define UI commands.

The input file name can be changed by using
/input/setFileName fileName

*/
void InputReader::SetCommands()
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
