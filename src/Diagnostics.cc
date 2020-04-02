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
/// \file Diagnostics.cc
/// \brief Implementation of the Diagnostics class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Diagnostics.hh"

#include "Units.hh"
#include "G4GenericMessenger.hh"
#include "G4ParticleTable.hh"

#include "G4VProcess.hh"

#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Retrieve analysis manager instance, initialize diagnostics numbers and call SetCommands.

*/
Diagnostics::Diagnostics(Units* units)
: fAnalysisManager(nullptr),
  fMessenger(nullptr),
  fParticleTable(nullptr),
  fElectron(nullptr),
  fGamma(nullptr),
  fPositron(nullptr),
  fUnits(units),
  fOutputFileBaseName("results"),
  fLowEnergyLimit(0.),
  fDiagSurfacePhaseSpaceActivation(false)
{
  fParticleTable = G4ParticleTable::GetParticleTable();

  // create analysis manager
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetVerboseLevel(1);
  G4cout << "Using " << fAnalysisManager->GetType()
         << " analysis manager" << G4endl;

  // get particles definition
  fElectron = G4Electron::Electron();
  fGamma    = G4Gamma::Gamma();
  fPositron = G4Positron::Positron();

  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Delete messenger.

*/
Diagnostics::~Diagnostics()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Initialize diagnostics by opening output files.

*/
void Diagnostics::InitializeAllDiags()
{
  // open output file
  fAnalysisManager->OpenFile(fOutputFileBaseName);

  fAnalysisManager->SetFirstNtupleId(0);
  fAnalysisManager->SetFirstNtupleColumnId(0);
  // fAnalysisManager->SetNtupleMerging(true);

  CreateDiagSurfacePhaseSpace();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Create a diagnostic that export the particle phase space.

The diagnostic is activated, and the correspondance between the given particle
and its Ntuple id is stored in the fDiagSurfacePhaseSpaceMap map.

*/
void Diagnostics::CreateDiagSurfacePhaseSpace()
{
  // create Ntuples
  fAnalysisManager->CreateNtuple("electron"   ,"Electron phase space");     // ID=0
  fAnalysisManager->CreateNtuple("gamma"      ,"Gamma phase space");        // ID=1
  fAnalysisManager->CreateNtuple("positron"   ,"Positron phase space");     // ID=3

  for (int i=0; i<3; i++) // loop over 3 Ntuples
  {
    fAnalysisManager->CreateNtupleDColumn(i, "Weight");
    fAnalysisManager->CreateNtupleDColumn(i, "x ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(i, "y ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(i, "z ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(i, "px ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(i, "py ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(i, "pz ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(i, "t ["+fUnits->GetTimeUnitLabel()+"]");

    fAnalysisManager->FinishNtuple(i);
  }
}

// methods to fill diagnostics

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Fill the particle phase space diagnostic at each layer surface.

*/
void Diagnostics::FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part, const G4StepPoint* stepPoint)
{
  // Test for filling the diag
  if (stepPoint->GetStepStatus() == fGeomBoundary &&            // The step is limited by the geometry
      stepPoint->GetKineticEnergy() > GetLowEnergyLimit())      // The particle's kinetic energy is higher than given threshold
  {
    // Get corresponding diag id
    G4int NtupleID=-1;

    if (part == fElectron) NtupleID=0;
    if (part == fGamma)    NtupleID=1;
    if (part == fPositron) NtupleID=2;

    // Get simulation units
    G4double rUnit = fUnits->GetPositionUnitValue();
    G4double pUnit = fUnits->GetMomentumUnitValue();
    G4double tUnit = fUnits->GetTimeUnitValue();

    // Get the particle properties
    G4double      w   = stepPoint->GetWeight();
    G4ThreeVector r   = stepPoint->GetPosition();
    G4ThreeVector p   = stepPoint->GetMomentum();
    G4double      t   = stepPoint->GetGlobalTime();

    // Fill the Ntuple
    if (NtupleID!=-1)
    {
      fAnalysisManager->FillNtupleDColumn(NtupleID,0,w); // weight by event
      fAnalysisManager->FillNtupleDColumn(NtupleID,1,r[0]/rUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,2,r[1]/rUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,3,r[2]/rUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,4,p[0]/pUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,5,p[1]/pUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,6,p[2]/pUnit);
      fAnalysisManager->FillNtupleDColumn(NtupleID,7,t/tUnit);

      fAnalysisManager->AddNtupleRow(NtupleID);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Write and close output files.

*/
void Diagnostics::FinishAllDiags()
{
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Define UI commands.

The input file name can be changed by using
/diags/setFileBaseName baseName
...
/diags/setLowEnergyLimit value unit
/diags/createDiagSurfacePhaseSpace particleName
/diags/createDiagVolumeEnergyDeposition particleName
/diags/createDiagVolumeProcess processName

*/
void Diagnostics::SetCommands()
{
  // get UI messenger
  fMessenger = new G4GenericMessenger(this,"/diags/","Manage simulation output");

  // define commands
  G4GenericMessenger::Command& setOutputFileBaseNameCmd
    = fMessenger->DeclareMethod("setFileBaseName",
                                &Diagnostics::SetOutputFileBaseName,
                                "Change output file name");


  G4GenericMessenger::Command& setLowEnergyLimitCmd
    = fMessenger->DeclarePropertyWithUnit("setLowEnergyLimit",
                                "MeV",
                                fLowEnergyLimit,
                                "Change low energy limit");


  // G4GenericMessenger::Command& createDiagSurfacePhaseSpaceCmd
  //   = fMessenger->DeclareMethod("createDiagSurfacePhaseSpace",
  //                               &Diagnostics::CreateDiagSurfacePhaseSpace,
  //                               "Activate phase space export at target interfaces, for given particle");

  // G4GenericMessenger::Command& createDiagVolumeEnergyDepositionCmd
  //   = fMessenger->DeclareMethod("createDiagVolumeEnergyDeposition",
  //                               &Diagnostics::CreateDiagVolumeEnergyDeposition,
  //                               "Activate energy deposition in target volume, for given particle");
  //
  // G4GenericMessenger::Command& createDiagVolumeProcessCmd
  //   = fMessenger->DeclareMethod("createDiagVolumeProcess",
  //                               &Diagnostics::CreateDiagVolumeProcess,
  //                               "Activate process export in target volume, for given process");

  // set commands properties
  setOutputFileBaseNameCmd.SetStates(G4State_Idle);

  setLowEnergyLimitCmd.SetStates(G4State_Idle);
  setLowEnergyLimitCmd.SetParameterName("lowE", true);
  setLowEnergyLimitCmd.SetRange("lowE>=0.");
  setLowEnergyLimitCmd.SetDefaultValue("0.");
  // setLowEnergyLimitCmd.SetUnitCategory("Energy");

  // createDiagSurfacePhaseSpaceCmd.SetStates(G4State_Idle);
  // createDiagVolumeEnergyDepositionCmd.SetStates(G4State_Idle);
  // createDiagVolumeProcessCmd.SetStates(G4State_Idle);
}
