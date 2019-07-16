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
/// \file Diagnostics.hh
/// \brief Definition of the Diagnostics class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Diagnostics_h
#define Diagnostics_h 1

#include "Analysis.hh"

class G4ParticleDefinition;
class G4ParticleTable;

#include "G4GenericMessenger.hh"

/**
\brief Creates and writes diagnostic output files.

*/
class Diagnostics:
{
  public:
    Diagnostics();
    ~Diagnostics();

    // user methods
    // methods to create diagnostics
    void CreateDiagSurfacePhaseSpace();
    // void CreateDiagSurfaceEnergySpectra();
    // void CreateDiagSurfaceThetaSpectra();
    // void CreateDiagSurfacePhiSpectra();
    // void CreateDiagVolumeEnergyDeposition();
    // void CreateDiagVolumeNuclearTransmutation();
    // void CreateDiagVolumeNeutronProduction();

    // methods to fill diagnostics
    void FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part,
                                  G4double weight,
                                  G4ThreeVector position,
                                  G4ThreeVector momentum,
                                  G4double time);
    // void FillDiagSurfaceEnergySpectra();
    // void FillDiagSurfaceThetaSpectra();
    // void FillDiagSurfacePhiSpectra();
    // void FillDiagVolumeEnergyDeposition();
    // void FillDiagVolumeNuclearTransmutation();
    // void FillDiagVolumeNeutronProduction();


    // methods to write output file
    void FinishAllDiags();
    void SetCommands();

    // get/set methods
    // methods to
    G4bool GetDiagSurfacePhaseSpaceActivation() {return fDiagSurfacePhaseSpaceActivation;};

    //
    G4GenericMessenger* GetMessenger() {return fMessenger;};
    G4double GetLowEnergyLimit() {return fLowEnergyLimit;};
    G4double GetHighEnergyLimit() {return fHighEnergyLimit;};

  private:
    G4AnalysisManager* fAnalysisManager; /**< \brief Pointer to the G4AnalysisManager instance.*/
    G4String fOutFileBaseName; /**< \brief Output file name.*/
    G4GenericMessenger* fMessenger; /**< \brief Pointer to the G4GenericMessenger instance for the output file.*/
    G4double fLowEnergyLimit;
    G4double fHighEnergyLimit;

    //
    G4bool fDiagSurfacePhaseSpaceActivation;

    G4ParticleTable* fParticleTable;
    std::map <G4ParticleDefinition part, G4int id> fDiagSurfacePhaseSpaceCounter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
