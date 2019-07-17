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
/// \file InputReader.hh
/// \brief Definition of the InputReader class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef InputReader_h
#define InputReader_h 1

class G4GenericMessenger;
class Units;
#include "globals.hh"
#include "G4ThreeVector.hh"

/**
\brief Read input file and interact with input macro-particles.

*/
class InputReader
{
  public:
    InputReader(Units* units);
    ~InputReader();

    // user methods
    void ReadInputFile();
    void NormalizeMacroParticlesWeights(G4int NumberOfEventsToBeProcessed);

    // get/set methods
    G4double GetMacroParticleWeight(G4int id) const {return fW[id];};
    G4ThreeVector GetMacroParticlePosition(G4int id) const {return G4ThreeVector(fX[id],fY[id],fZ[id]);};
    G4ThreeVector GetMacroParticleMomentum(G4int id) const {return G4ThreeVector(fPx[id],fPy[id],fPz[id]);};
    G4double GetMacroParticleTime(G4int id) const {return fT[id];};

    G4int GetNumberOfMacroParticles() const {return fW.size();};

    G4String GetParticleName() {return fParticleName;};

    void SetInputFileName(G4String inputFileName) {fInputFileName = inputFileName;};
    void SetParticleName(G4String particleName) {fParticleName = particleName;};

    void SetCommands();

  private:
    // Geant4 pointers
    G4GenericMessenger* fMessenger; /**< \brief Pointer to the G4GenericMessenger instance for the input file.*/

    // User pointers
    Units* fUnits; /**< \brief Pointer to the Units instance.*/

    // User variables
    G4String fInputFileName; /**< \brief Input file name.*/
    G4String fParticleName; /**< \brief Input particle name.*/

    std::vector<G4double> fW,fX,fY,fZ,fPx,fPy,fPz,fT;  /**< \brief Arrays containing input macro-particles characteristics.*/
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
