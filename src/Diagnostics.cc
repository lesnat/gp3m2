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
  fLowEnergyLimit(0.),
  fHighEnergyLimit(10000000000),
  fDiagSurfacePhaseSpaceActivation(false)
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
and its Ntuple id is stored in the fDiagSurfacePhaseSpaceCounter map.

*/
void Diagnostics::CreateDiagSurfacePhaseSpace(G4String particle)
{
  // activate surface phase space diagnostic
  fDiagSurfacePhaseSpaceActivation = true;

  // create Ntuples
  G4int currentNtupleId = fDiagSurfacePhaseSpaceCounter.Size();

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
  fDiagSurfacePhaseSpaceCounter[fParticleTable->FindParticle(particle)] = currentNtupleId;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagSurfaceEnergySpectra() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagSurfaceThetaSpectra() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagSurfacePhiSpectra() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagVolumeEnergyDeposition() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagVolumeNuclearTransmutation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::CreateDiagVolumeNeutronProduction() {}

// methods to fill diagnostics

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Fill the particle phase space diagnostic at each layer surface.

*/
void Diagnostics::FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part, const G4StepPoint* stepPoint)
{
  G4int currentNtupleId = fDiagSurfacePhaseSpaceCounter[part];

  if (stepPoint->GetStepStatus() == fGeomBoundary &&            // The step is limited by the geometry
      stepPoint->GetKineticEnergy() > GetLowEnergyLimit() &&    // The particle's kinetic energy is higher than given threshold
      stepPoint->GetKineticEnergy() < GetHighEnergyLimit())     // The particle's kinetic energy is lower than given threshold
  {
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagSurfaceEnergySpectra(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagSurfaceThetaSpectra(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagSurfacePhiSpectra(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagVolumeEnergyDeposition(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void Diagnostics::FillDiagVolumeNuclearTransmutation(const G4ParticleDefinition* part, const G4Step* stepPoint)
// {}

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
/diags/setLowEnergyLimit value unit
/diags/setHighEnergyLimit value unit
/diags/createDiagSurfacePhaseSpace particleName

*/
void Diagnostics::SetCommands()
{
  // get UI messenger
  fMessenger = new G4GenericMessenger(this,"/diags/","Manage simulation output");

  // define commands
  G4GenericMessenger::Command& setOutputFileBaseNameCmd
    = fMessenger->DeclareProperty("setFileBaseName",
                                fOutputFileBaseName,
                                "Change output file name");

  G4GenericMessenger::Command& setLowEnergyLimitCmd
    = fMessenger->DeclarePropertyWithUnit("setLowEnergyLimit",
                                "MeV",
                                fLowEnergyLimit,
                                "Change low energy limit");

  G4GenericMessenger::Command& setHighEnergyLimitCmd
    = fMessenger->DeclarePropertyWithUnit("setHighEnergyLimit",
                                "MeV",
                                fHighEnergyLimit,
                                "Change low energy limit");

  G4GenericMessenger::Command& createDiagSurfacePhaseSpaceCmd
    = fMessenger->DeclareMethod("createDiagSurfacePhaseSpace",
                                &Diagnostics::CreateDiagSurfacePhaseSpace,
                                "Add a new layer to the target");

  // set commands properties
  setOutputFileBaseNameCmd.SetStates(G4State_Idle);

  setLowEnergyLimitCmd.SetStates(G4State_Idle);
  setLowEnergyLimitCmd.SetParameterName("lowE", true);
  setLowEnergyLimitCmd.SetRange("lowE>=0.");
  setLowEnergyLimitCmd.SetDefaultValue("0.");
  // setLowEnergyLimitCmd.SetUnitCategory("Energy");

  setHighEnergyLimitCmd.SetStates(G4State_Idle);
  setHighEnergyLimitCmd.SetParameterName("lowE", true);
  setHighEnergyLimitCmd.SetRange("lowE>=0.");
  setHighEnergyLimitCmd.SetDefaultValue("1e10");
  // setHighEnergyLimitCmd.SetUnitCategory("Energy");

  createDiagSurfacePhaseSpaceCmd.SetStates(G4State_Idle);
}
