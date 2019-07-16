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
/// \file RunAction.hh
/// \brief Definition of the RunAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "Analysis.hh"
#include "G4ThreeVector.hh"

class G4Run;
class G4ParticleDefinition;
class Diagnostics;
#include "G4GenericMessenger.hh"

/**
\brief Creates and writes diagnostic output files.

*/
class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    // base class methods
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void   EndOfRunAction(const G4Run*);

    // user methods
    void FillData(const G4ParticleDefinition* part,
                  G4double weight,
                  G4ThreeVector position,
                  G4ThreeVector momentum,
                  G4double time);
    void SetCommands();
    void ReadInput(G4int NbOfEvents);

    // get/set methods
    G4double GetEntry(G4String variable, G4int id) const
    { if      (variable=="w")  return fW[id];
      else if (variable=="x")  return fX[id];
      else if (variable=="y")  return fY[id];
      else if (variable=="z")  return fZ[id];
      else if (variable=="px") return fPx[id];
      else if (variable=="py") return fPy[id];
      else if (variable=="pz") return fPz[id];
      else if (variable=="t")  return fT[id];
      else G4cerr << "Unknown variable name in RunAction::GetEntry."<< G4endl; throw;};
    G4int GetLength() const {return fW.size();};
    G4GenericMessenger* GetInMessenger() {return fInMessenger;};
    Diagnostics* GetDiagnostics() {return fDiagnostics;};

  private:
    G4AnalysisManager* fAnalysisManager; /**< \brief Pointer to the G4AnalysisManager instance.*/
    G4String fInFileName; /**< \brief Input file name.*/
    G4GenericMessenger* fInMessenger; /**< \brief Pointer to the G4GenericMessenger instance for the input file.*/
    Diagnostics* fDiagnostics;

    std::vector<G4double> fW,fX,fY,fZ,fPx,fPy,fPz,fT;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
