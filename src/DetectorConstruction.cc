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

#include "Units.hh"
#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4GenericMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Initialize pointers, set default values and creates UI commands.

*/
DetectorConstruction::DetectorConstruction(Units* units)
: G4VUserDetectorConstruction(),
  fMessenger(nullptr),
  fWorldLV(nullptr),
  fUnits(units),
  fNumberOfLayers(0),
  fPropagationAxis(""),
  fTargetSizeLongi(0),
  fTargetRadius(5*cm),
  fCheckOverlaps(true)
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
  G4double worldXYZ  = 1*m;
  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");

  // Create World solid
  G4Box* worldS =
    new G4Box("WorldS",                       // name
              worldXYZ/2,                     // size X
              worldXYZ/2,                     // size Y
              worldXYZ/2);                    // size Z

  // Create World logical volume
  fWorldLV =
    new G4LogicalVolume(worldS,               // solid
                        worldMat,             // material
                        "WorldLV");           // name

  // Create World physical volume
  G4VPhysicalVolume* worldPV =
    new G4PVPlacement(0,                      // no rotation
                      G4ThreeVector(),        // at (0,0,0)
                      fWorldLV,               // logical volume
                      "World",                // name
                      0,                      // mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      fCheckOverlaps);        // overlaps checking

  //always return the World physical volume
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddTargetLayer(G4String materialName,
                                          G4double targetWidth)
{
  // Retrieve the new layer material
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* layerMat = nist->FindOrBuildMaterial(materialName);

  // Test if the material was properly constructed
  if (layerMat == nullptr)
  {
    G4cerr << "Unknown material : " << materialName << G4endl;
    throw;
  }

  // Get layer longitudinal size;
  G4double width = targetWidth * fUnits->GetPositionUnitValue();

  // Matrix to rotate the cylinders in the fPropagationAxis direction
  G4RotationMatrix* rotation = new G4RotationMatrix();
  if (fPropagationAxis == "x") {
    rotation->rotateY(90. * deg);
  } else if (fPropagationAxis == "y") {
    rotation->rotateX(90. * deg);
  } else if (fPropagationAxis == "z") {
    ; // Cylinder is already oriented along the z axis
  }

  // Create layer solid volume
  G4Tubs* layerS =
    new G4Tubs("layerS",                    // name
            0.,                             // inner radius
            fTargetRadius,                  // outer radius
            width/2.,                       // z-half length
            0.,                             // starting Phi
            twopi);                         // segment angle


  // Create Layer logical volume
  G4LogicalVolume* layerLV =
    new G4LogicalVolume(layerS,              // solid
                        layerMat,            // material
                        "LayerLV");          // name

  // New layer position
  G4ThreeVector position;
  position = G4ThreeVector(fTargetSizeLongi + width/2.,0,0);

  // Create Layer physical volume
  new G4PVPlacement(rotation,              // rotation
                    position,              // at (0,0,0)
                    layerLV,               // logical volume
                    "Layer",               // name
                    fWorldLV,              // mother  volume
                    false,                 // no boolean operation
                    fNumberOfLayers,       // copy number
                    fCheckOverlaps);       // overlaps checking

  // Update target size and number of layers
  fTargetSizeLongi += width;
  fNumberOfLayers++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Set commands to be interpreted with the UI

The AddTargetLayer function can be called in UI in the following way :

/target/addLayer materialName width
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
  G4GenericMessenger::Command& setTargetRadiusCmd
    = fMessenger->DeclareMethod("setRadius",
                                &DetectorConstruction::SetTargetRadius,
                                "Set target radius");
  G4GenericMessenger::Command& setPropagationAxisCmd
    = fMessenger->DeclareMethod("setPropagationAxis",
                                &DetectorConstruction::SetPropagationAxis,
                                "Make the targer layers being oriented along the propagation axis");
  // set commands properties
  setTargetRadiusCmd.SetStates(G4State_Idle);
  setPropagationAxisCmd.SetStates(G4State_Idle);
  addLayerCmd.SetStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
