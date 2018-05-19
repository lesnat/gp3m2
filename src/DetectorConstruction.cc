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

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief

*/
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fWorldLV(0),
  fTargetSizeYZ(1*mm),
  fTargetSizeX(0),
  fCheckOverlaps(true)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief

*/
DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Construct the geometry

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

  AddTargetLayer("G4_Al",50*um);
  AddTargetLayer("G4_W",50*um);
  AddTargetLayer("G4_W",50*um);

  //always return the World physical volume
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::AddTargetLayer(G4String materialName, G4double width)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* layerMat = nist->FindOrBuildMaterial(materialName);
  // G4bool checkOverlaps = true;

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
                    0,                     // copy number
                    fCheckOverlaps);       // overlaps checking

  fTargetSizeX += width;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
