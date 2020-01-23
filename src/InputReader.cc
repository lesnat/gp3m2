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
#include "Units.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Call the SetCommands method.

*/
InputReader::InputReader(Units* units)
: fMessenger(nullptr),
  fUnits(units),
  fInputFileName(""),
  fParticleName("geantino")
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
\brief Read the input phase space, and save macro-particles characteristics into arrays.

The input file format must be a list of macro-particles:
w   x   y   z   px  py  pz  t
w   x   y   z   px  py  pz  t
w   x   y   z   px  py  pz  t
w   x   y   z   px  py  pz  t

with separators being spaces.
*/
void InputReader::ReadInputFile()
{

  // Clear fW,fX,... before import
  fW.clear()    ;
  fX.clear()    ; fY.clear()  ; fZ.clear();
  fPx.clear()   ; fPy.clear() ; fPz.clear();
  fT.clear()    ;

  // Get simulation units
  G4double rUnit = fUnits->GetPositionUnitValue();
  G4double pUnit = fUnits->GetMomentumUnitValue();
  G4double tUnit = fUnits->GetTimeUnitValue();

  // Define streams
  std::ifstream input;
  std::string str;
  input.open(fInputFileName);

  // Declare temporary variables
  G4double w,x,y,z,px,py,pz,t;

  // Read file
  while (!input.eof())
  {
    // Get lines, ignore comments and empty lines
    std::getline(input,str);
    if(str[0]=='#' or str=="") continue;
    std::stringstream ss(str);

    // Save input data in code units
    ss >> w >> x >> y >> z >> px >> py >> pz >> t;
    fW.push_back(w)         ;
    fX.push_back(x*rUnit)   ; fY.push_back(y*rUnit)  ; fZ.push_back(z*rUnit);
    fPx.push_back(px*pUnit) ; fPy.push_back(py*pUnit); fPz.push_back(pz*pUnit);
    fT.push_back(t*tUnit)   ;
  }
  input.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Normalize macro-particles weights in order to conserve total number of particles

*/
void InputReader::NormalizeMacroParticlesWeights(G4int NumberOfEventsToBeProcessed)
{
  G4int NumberOfMacroParticles = fW.size();
  G4double normW = (G4double)NumberOfEventsToBeProcessed/(G4double)NumberOfMacroParticles;
  for (int i=0; i<NumberOfMacroParticles; i++)
  {
    fW[i] = fW[i]/normW;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Define UI commands.

The input file name can be changed by using
/input/setFileName fileName
/input/setParticle particleName

*/
void InputReader::SetCommands()
{
  // get UI messengers
  fMessenger = new G4GenericMessenger(this,"/input/","Manage simulation input");

  // define commands
  G4GenericMessenger::Command& setInputFileNameCmd
    = fMessenger->DeclareMethod("setFileName",
                                &InputReader::SetInputFileName,
                                "Change input file name");

  G4GenericMessenger::Command& setParticleNameCmd
    = fMessenger->DeclareMethod("setParticle",
                              &InputReader::SetParticleName,
                              "Change particle type");

  // set commands properties
  setParticleNameCmd.SetStates(G4State_Idle);
  setInputFileNameCmd.SetStates(G4State_Idle);
}
