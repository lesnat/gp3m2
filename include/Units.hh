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
/// \file Units.hh
/// \brief Definition of the Units class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Units_h
#define Units_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4GenericMessenger;

/**
\brief Define units of the input and output files

*/
class Units
{
  public:
    Units();
    ~Units();

    // get/set methods
    G4String GetPositionUnitLabel() {return fPositionUnitLabel;};
    G4String GetMomentumUnitLabel() {return fMomentumUnitLabel;};
    G4String GetTimeUnitLabel() {return fTimeUnitLabel;};

    G4double GetPositionUnitValue()
    {
      if (fPositionUnitLabel == "nm"){
        return nm;
      } else if(fPositionUnitLabel == "um"){
        return um;
      } else if(fPositionUnitLabel == "mm"){
        return mm;
      } else if(fPositionUnitLabel == "cm"){
        return cm;
      } else if(fPositionUnitLabel == "m"){
        return m;
      } else {
        G4cerr << "Unknown position unit : " << fPositionUnitLabel << G4endl;
        throw;
      }
    };

    G4double GetMomentumUnitValue()
    {
      if (fMomentumUnitLabel == "eV"){
        return eV;
      } else if (fMomentumUnitLabel == "keV") {
        return keV;
      } else if (fMomentumUnitLabel == "MeV") {
        return MeV;
      } else if (fMomentumUnitLabel == "GeV") {
        return GeV;
      } else if (fMomentumUnitLabel == "TeV") {
        return TeV;
      } else {
        G4cerr << "Unknown momentum unit : " << fMomentumUnitLabel << G4endl;
        throw;
      }
    };

    G4double GetTimeUnitValue()
    {
      if (fTimeUnitLabel == "fs"){
        return (0.001 * ps);
      } else if (fTimeUnitLabel == "ps") {
        return ps;
      } else if (fTimeUnitLabel == "ns") {
        return ns;
      } else if (fTimeUnitLabel == "us") {
        return us;
      } else if (fTimeUnitLabel == "ms") {
        return ms;
      } else if (fTimeUnitLabel == "s") {
        return s;
      } else {
        G4cerr << "Unknown time unit : " << fTimeUnitLabel << G4endl;
        throw;
      }
    };

    void SetPositionUnit(G4String positionUnitLabel) {fPositionUnitLabel = positionUnitLabel;};
    void SetMomentumUnit(G4String momentumUnitLabel) {fMomentumUnitLabel = momentumUnitLabel;};
    void SetTimeUnit(G4String timeUnitLabel) {fTimeUnitLabel = timeUnitLabel;};

    void SetCommands();

  private:
    // Geant4 pointers
    G4GenericMessenger* fMessenger;

    // User pointers

    // User variables
    G4String fPositionUnitLabel;
    G4String fMomentumUnitLabel;
    G4String fTimeUnitLabel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
