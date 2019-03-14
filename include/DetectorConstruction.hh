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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4GenericMessenger;
/**
\brief Construct geometry

This class is shared and instanciated only once
*/
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    // base class methods
    virtual G4VPhysicalVolume* Construct();

    // user methods
    void AddTargetLayer(G4String materialName, G4double width_um);
    void SetCommands();

  private:
    G4int fNumberOfLayers; /**< \brief Total number of layers in the target.*/
    G4bool fCheckOverlaps; /**< \brief Check if volumes are overlapping.*/
    G4double fTargetSizeX; /**< \brief Total target longitudinal size.*/
    G4double fTargetSizeYZ; /**< \brief Target transverse size.*/
    G4LogicalVolume* fWorldLV; /**< \brief Pointer to the world logical volume.*/
    G4GenericMessenger* fMessenger; /**< \brief Pointer to the G4GenericMessenger instance.*/
};

#endif
