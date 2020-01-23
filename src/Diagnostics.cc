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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Retrieve analysis manager instance, initialize diagnostics numbers and call SetCommands.

*/
Diagnostics::Diagnostics(Units* units)
: fAnalysisManager(nullptr),
  fMessenger(nullptr),
  fParticleTable(nullptr),
  fUnits(units),
  fOutputFileBaseName("results"),
  fLowEnergyLimit(0.)
{
  fParticleTable = G4ParticleTable::GetParticleTable();

  // create analysis manager
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetVerboseLevel(1);
  G4cout << "Using " << fAnalysisManager->GetType()
         << " analysis manager" << G4endl;

  fAnalysisManager->SetFirstNtupleId(0);
  fAnalysisManager->SetFirstNtupleColumnId(0);
  // fAnalysisManager->SetNtupleMerging(true);

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Create a diagnostic that export the particle phase space.

The diagnostic is activated, and the correspondance between the given particle
and its Ntuple id is stored in the fDiagSurfacePhaseSpaceMap map.

*/
void Diagnostics::CreateDiagSurfacePhaseSpace(G4String particle)
{
  // Test if the particle exists
  if (fParticleTable->contains(particle)) {

    // create Ntuples
    G4int currentNtupleId = fDiagSurfacePhaseSpaceMap.Size();

    fAnalysisManager->CreateNtuple(particle , particle + " phase space");

    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "Weight");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "x ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "y ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "z ["+fUnits->GetPositionUnitLabel()+"]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "px ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "py ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "pz ["+fUnits->GetMomentumUnitLabel()+"/c]");
    fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "t ["+fUnits->GetTimeUnitLabel()+"]");

    fAnalysisManager->FinishNtuple(currentNtupleId);

    // save correspondance between particle and Ntuple ID
    fDiagSurfacePhaseSpaceMap.Insert(fParticleTable->FindParticle(particle), currentNtupleId);
  } else {
    G4cerr << "Unknown particle : " << particle << G4endl;
    throw;
  }
}

// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagVolumeEnergyDeposition(G4String particle)
// {
//   //
// }
//
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagVolumeProcess(const G4String process)
// {
//   // create Ntuples
//   G4int currentNtupleId = fDiagVolumeProcessMap.Size();
//
//   fAnalysisManager->CreateNtuple(process , process + " location");
//
//   fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "Weight");
//   fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "x ["+fUnits->GetPositionUnitLabel()+"]");
//   fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "y ["+fUnits->GetPositionUnitLabel()+"]");
//   fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "z ["+fUnits->GetPositionUnitLabel()+"]");
//   fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "t ["+fUnits->GetTimeUnitLabel()+"]");
//
//   fAnalysisManager->FinishNtuple(currentNtupleId);
//
//   // save correspondance between particle and Ntuple ID
//   fDiagVolumeProcessMap[process] = currentNtupleId;
// }

// methods to fill diagnostics

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Fill the particle phase space diagnostic at each layer surface.

*/
void Diagnostics::FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part, const G4StepPoint* stepPoint)
{
  // Test for filling the diag
  if (fDiagSurfacePhaseSpaceMap.Has(part) &&                    // The diag has been activated for this particle
      stepPoint->GetStepStatus() == fGeomBoundary &&            // The step is limited by the geometry
      stepPoint->GetKineticEnergy() > GetLowEnergyLimit())      // The particle's kinetic energy is higher than given threshold
  {
    // Get corresponding diag id
    G4int currentNtupleId = fDiagSurfacePhaseSpaceMap[part];

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
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,0,w); // weight by event
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,1,r[0]/rUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,2,r[1]/rUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,3,r[2]/rUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,4,p[0]/pUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,5,p[1]/pUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,6,p[2]/pUnit);
    fAnalysisManager->FillNtupleDColumn(currentNtupleId,7,t/tUnit);

    fAnalysisManager->AddNtupleRow(currentNtupleId);
  }
}

// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagVolumeEnergyDeposition(const G4ParticleDefinition* part, const G4StepPoint* stepPoint)
// {
//   //
// }
//
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagVolumeProcess(const G4String process, const G4StepPoint* stepPoint)
// {
//   G4int stepStatus = stepPoint->GetStepStatus();
//
//   if (fDiagVolumeProcessMap.Has(process) &&                     // The diag has been activated for this process
//       (stepStatus == fAlongStepDoItProc ||                      // The particle's kinetic energy is higher than given threshold
//       stepStatus == fPostStepDoItProc ||                        // The particle's kinetic energy is higher than given threshold
//       stepStatus == fExclusivelyForcedProc) &&                  // The particle's kinetic energy is higher than given threshold
//       stepPoint->GetProcessDefinedStep()->GetProcessName() == process)// The step is limited by the right process
//   {
//     // Get corresponding diag id
//     G4int currentNtupleId = fDiagVolumeProcessMap[process];
//
//     // Get simulation units
//     G4double rUnit = fUnits->GetPositionUnitValue();
//     G4double tUnit = fUnits->GetTimeUnitValue();
//
//     // Get the particle properties
//     G4double      w   = stepPoint->GetWeight();
//     G4ThreeVector r   = stepPoint->GetPosition();
//     G4double      t   = stepPoint->GetGlobalTime();
//
//     // Fill the Ntuple
//     fAnalysisManager->FillNtupleDColumn(currentNtupleId,0,w); // weight by event
//     fAnalysisManager->FillNtupleDColumn(currentNtupleId,1,r[0]/rUnit);
//     fAnalysisManager->FillNtupleDColumn(currentNtupleId,2,r[1]/rUnit);
//     fAnalysisManager->FillNtupleDColumn(currentNtupleId,3,r[2]/rUnit);
//     fAnalysisManager->FillNtupleDColumn(currentNtupleId,7,t/tUnit);
//
//     fAnalysisManager->AddNtupleRow(currentNtupleId);
//   }
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagVolumeNeutronProduction(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}


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


  G4GenericMessenger::Command& createDiagSurfacePhaseSpaceCmd
    = fMessenger->DeclareMethod("createDiagSurfacePhaseSpace",
                                &Diagnostics::CreateDiagSurfacePhaseSpace,
                                "Activate phase space export at target interfaces, for given particle");

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

  createDiagSurfacePhaseSpaceCmd.SetStates(G4State_Idle);
  // createDiagVolumeEnergyDepositionCmd.SetStates(G4State_Idle);
  // createDiagVolumeProcessCmd.SetStates(G4State_Idle);
}
