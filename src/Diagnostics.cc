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
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4ParticleTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
\brief Create analysis manager, Ntuples and set UI commands.

*/
Diagnostics::Diagnostics()
: fOutFileBaseName("results"),
  fMessenger(nullptr),
  fLowEnergyLimit(0.),
  fHighEnergyLimit(10000000000),
  fParticleTable(nullptr),
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
Diagnostics::~Diagnostics()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Diagnostics::InitializeAllDiags()
{
  // open output file
  fAnalysisManager->OpenFile(fOutFileBaseName);
}

// methods to create diagnostics

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Diagnostics::CreateDiagSurfacePhaseSpace(G4String particle)
{
  // create Ntuples
  G4int currentNtupleId = fDiagSurfacePhaseSpaceCounter.Size();

  fAnalysisManager->CreateNtuple(particle , particle + " phase space");

  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "Weight");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "x0_um");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "x1_um");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "x2_um");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "p0_MeV");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "p1_MeV");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "p2_MeV");
  fAnalysisManager->CreateNtupleDColumn(currentNtupleId, "time_fs");

  fAnalysisManager->FinishNtuple(currentNtupleId);

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
void Diagnostics::FillDiagSurfacePhaseSpace(const G4ParticleDefinition* part, const G4Step* stepPoint)
{
  G4int currentNtupleID = fDiagSurfacePhaseSpaceCounter[part];

  if (stepPoint->GetStepStatus() == fGeomBoundary &&            // The step is limited by the geometry
      stepPoint->GetKineticEnergy() > GetLowEnergyLimit() &&    // The particle's kinetic energy is higher than given threshold
      stepPoint->GetKineticEnergy() < GetHighEnergyLimit())     // The particle's kinetic energy is lower than given threshold
  {
    G4double      weight    = stepPoint->GetWeight();
    G4ThreeVector position  = stepPoint->GetPosition();
    G4ThreeVector momentum  = stepPoint->GetMomentum();
    G4double      time      = stepPoint->GetGlobalTime();

    fAnalysisManager->FillNtupleDColumn(currentNtupleID,0,weight); // weight by event
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,1,position[0]/um);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,2,position[1]/um);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,3,position[2]/um);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,4,momentum[0]/MeV);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,5,momentum[1]/MeV);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,6,momentum[2]/MeV);
    fAnalysisManager->FillNtupleDColumn(currentNtupleID,7,time/(1e-3*ps));

    fAnalysisManager->AddNtupleRow(currentNtupleID);
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


// methods to write output file

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Diagnostics::FinishAllDiags()
{
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Diagnostics::SetCommands()
{
  // get UI messengers
  fMessenger = new G4GenericMessenger(this,"/diags/","Manage simulation output");

  // define commands
  G4GenericMessenger::Command& setOutFileBaseNameCmd
    = fMessenger->DeclareProperty("setFileBaseName",
                                fOutFileBaseName,
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
  setOutFileBaseNameCmd.SetStates(G4State_Idle);

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
}
