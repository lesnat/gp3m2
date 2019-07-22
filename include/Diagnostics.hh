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

// selection of the analysis manager output format
// #include "g4root.hh"
// #include "g4xml.hh"
#include "g4csv.hh"

class G4ParticleDefinition;
class G4ParticleTable;
class Units;

#include "G4GenericMessenger.hh"
#include "G4Cache.hh"
#include "G4StepPoint.hh"

/**
\brief Creates and writes diagnostic output files.

*/
class Diagnostics
{
  public:
    Diagnostics(Units* units);
    ~Diagnostics();

    // user methods
    // methods to create diagnostics
    void CreateDiagSurfacePhaseSpace(G4String particle);
    // void CreateDiagSurfaceEnergySpectra();
    // void CreateDiagSurfaceThetaSpectra();
    // void CreateDiagSurfacePhiSpectra();
    // void CreateDiagVolumeEnergyDeposition();
    // void CreateDiagVolumeNuclearTransmutation();
    // void CreateDiagVolumeNeutronProduction();

    // methods to fill diagnostics
    void FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagSurfaceEnergySpectra(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagSurfaceThetaSpectra(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagSurfacePhiSpectra(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagVolumeEnergyDeposition(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagVolumeNuclearTransmutation(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);
    // void FillDiagVolumeNeutronProduction(const G4ParticleDefinition* part, const G4StepPoint* stepPoint);


    // methods to write output file
    void InitializeAllDiags();
    void FinishAllDiags();
    void SetCommands();

    // get/set methods
    // methods to retrieve diag activation
    G4bool GetDiagSurfacePhaseSpaceActivation() {return fDiagSurfacePhaseSpaceActivation;};
    void SetOutputFileBaseName(G4String outputFileBaseName) {fOutputFileBaseName = outputFileBaseName;};

    // methods to retrieve low and high energy limits
    G4double GetLowEnergyLimit() {return fLowEnergyLimit;};
    G4double GetHighEnergyLimit() {return fHighEnergyLimit;};

  private:
    // Geant4 pointers
    G4AnalysisManager* fAnalysisManager; /**< \brief Pointer to the G4AnalysisManager instance.*/
    G4GenericMessenger* fMessenger; /**< \brief Pointer to the G4GenericMessenger instance for the output file.*/
    G4ParticleTable* fParticleTable; /**< \brief Pointer to the G4ParticleTable instance.*/

    // User pointers
    Units* fUnits; /**< \brief Pointer to the Units instance.*/

    // User variables
    G4String fOutputFileBaseName; /**< \brief Output file base name.*/
    G4double fLowEnergyLimit; /**< \brief Lower energy to fill diagnostics.*/
    G4double fHighEnergyLimit; /**< \brief Higher energy to fill diagnostics.*/
    G4bool fDiagSurfacePhaseSpaceActivation;  /**< \brief .*/

    G4MapCache <const G4ParticleDefinition*, G4int> fDiagSurfacePhaseSpaceCounter;  /**< \brief .*/
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
