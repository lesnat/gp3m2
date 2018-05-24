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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4GenericMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Initialize pointers, set default values and creates UI commands.

*/
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fNumberOfLayers(0),
  fCheckOverlaps(true),
  fTargetSizeX(0),
  fTargetSizeYZ(1*mm),
  fWorldLV(0),
  fMessenger(0)
{
  // set UI commands
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete messenger.

*/
DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Construct the default World volume

The world is defined as a 0.5 x 1 x 1 cm box of G4_Galactic material.
*/
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Define World default properties
  G4double worldX  = 0.5*cm;
  G4double worldYZ = 1.*cm;
  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");

  // Create World solid
  G4Box* worldS =
    new G4Box("WorldS",                      // name
              0.5*worldX,                    // size X
              0.5*worldYZ,                   // size Y
              0.5*worldYZ);                  // size Z

  // Create World logical volume
  fWorldLV =
    new G4LogicalVolume(worldS,              // solid
                        worldMat,            // material
                        "WorldLV");          // name

  // Create World physical volume
  G4VPhysicalVolume* worldPV =
    new G4PVPlacement(0,                     // no rotation
                      G4ThreeVector(),       // at (0,0,0)
                      fWorldLV,               // logical volume
                      "World",               // name
                      0,                     // mother  volume
                      false,                 // no boolean operation
                      0,                     // copy number
                      fCheckOverlaps);       // overlaps checking

  //always return the World physical volume
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Add a new layer to the target.

\param materialName : Material name of the new layer
\param width_um : Width of the new layer (in um)

The transverse size of the new layer is always the same default value.

The new width is added to fTargetSizeX and fNumberOfLayers is incremented.
*/
void DetectorConstruction::AddTargetLayer(G4String materialName,
                                          G4double width_um)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* layerMat = nist->FindOrBuildMaterial(materialName);
  // G4bool checkOverlaps = true;

  G4double width = width_um * um;

  // Create Layer solid
  G4Box* layerS =
    new G4Box("LayerS",                      // name
              width/2,                       // size X
              fTargetSizeYZ/2,               // size Y
              fTargetSizeYZ/2);              // size Z

  // Create Layer logical volume
  G4LogicalVolume* layerLV =
    new G4LogicalVolume(layerS,              // solid
                        layerMat,            // material
                        "LayerLV");          // name

  // New layer position
  G4ThreeVector position;
  if (fTargetSizeX==0.)
  {
    position = G4ThreeVector(width/2,0,0);
    fTargetSizeX += width/2;
  }
  else
  {
    position = G4ThreeVector(fTargetSizeX,0,0);
  }

  // Create Layer physical volume
  new G4PVPlacement(0,                     // no rotation
                    position,              // at (0,0,0)
                    layerLV,               // logical volume
                    "Layer",               // name
                    fWorldLV,              // mother  volume
                    false,                 // no boolean operation
                    fNumberOfLayers,       // copy number
                    fCheckOverlaps);       // overlaps checking

  fTargetSizeX += width;
  fNumberOfLayers++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Set commands to be interpreted with the UI

The AddTargetLayer function can be called in UI in the following way :

/target/addLayer materialName width_um
*/
void DetectorConstruction::SetCommands()
{
  // get UI messengers
  fMessenger = new G4GenericMessenger(this,"/target/","Manage target layers");

  // define commands
  G4GenericMessenger::Command& addLayerCmd
    = fMessenger->DeclareMethod("addLayer",
                                &DetectorConstruction::AddTargetLayer,
                                "Add a new layer to the target");
                                
  // set commands properties
  addLayerCmd.SetStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
