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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "G4RunManager.hh" // includes all the needed classes for RunAction

#include "G4SystemOfUnits.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Create analysis manager, Ntuples and set UI commands.

*/
RunAction::RunAction()
: G4UserRunAction(),
  fOutFileName("results"),
  fInFileName(""),
  fGamma(0),
  fElectron(0),
  fPositron(0),
  fOutMessenger(0),
  fInMessenger(0)
{
  // get particles definition
  fGamma    = G4Gamma::Gamma();
  fElectron = G4Electron::Electron();
  fPositron = G4Positron::Positron();

  // create analysis manager
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetVerboseLevel(1);
  G4cout << "Using " << fAnalysisManager->GetType()
         << " analysis manager" << G4endl;


  // create Ntuples
  fAnalysisManager->SetFirstNtupleId(0);
  fAnalysisManager->SetFirstNtupleColumnId(0);
  // fAnalysisManager->SetNtupleMerging(true);

  fAnalysisManager->CreateNtuple("electron"   ,"Electron phase space");     // ID=0
  fAnalysisManager->CreateNtuple("gamma"      ,"Gamma phase space");        // ID=1
  fAnalysisManager->CreateNtuple("positron"   ,"Positron phase space");     // ID=3

  for (int i=0; i<3; i++) // loop over 3 Ntuples
  {
    fAnalysisManager->CreateNtupleDColumn(i, "Weight");
    fAnalysisManager->CreateNtupleDColumn(i, "x0_um");
    fAnalysisManager->CreateNtupleDColumn(i, "x1_um");
    fAnalysisManager->CreateNtupleDColumn(i, "x2_um");
    fAnalysisManager->CreateNtupleDColumn(i, "p0_MeV");
    fAnalysisManager->CreateNtupleDColumn(i, "p1_MeV");
    fAnalysisManager->CreateNtupleDColumn(i, "p2_MeV");
    fAnalysisManager->CreateNtupleDColumn(i, "time_fs");

    fAnalysisManager->FinishNtuple(i);
  }

  // set UI commands
  SetCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Delete analysis manager.

*/
RunAction::~RunAction()
{
  delete fAnalysisManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Read input file & open output file.

This user code is executed at the beginning of each run
*/
void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // read input file
  ReadInput();
  
  // open output file
  fAnalysisManager->OpenFile(fOutFileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Write and close output file

This user code is executed at the end of each run
*/
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save Ntuples
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief Add a new line to the corresponding Ntuple.

All the quantities must be given in code units.
*/
void RunAction::FillData(const G4ParticleDefinition* part,
                         G4double weight,
                         G4ThreeVector position,
                         G4ThreeVector momentum,
                         G4double time)
{
  G4int NtupleID=-1;

  if (part == fElectron) NtupleID=0;
  if (part == fGamma)    NtupleID=1;
  if (part == fPositron) NtupleID=2;

  if (NtupleID!=-1)
  {
    fAnalysisManager->FillNtupleDColumn(NtupleID,0,weight); // weight by event
    fAnalysisManager->FillNtupleDColumn(NtupleID,1,position[0]/um);
    fAnalysisManager->FillNtupleDColumn(NtupleID,2,position[1]/um);
    fAnalysisManager->FillNtupleDColumn(NtupleID,3,position[2]/um);
    fAnalysisManager->FillNtupleDColumn(NtupleID,4,momentum[0]/MeV);
    fAnalysisManager->FillNtupleDColumn(NtupleID,5,momentum[1]/MeV);
    fAnalysisManager->FillNtupleDColumn(NtupleID,6,momentum[2]/MeV);
    fAnalysisManager->FillNtupleDColumn(NtupleID,7,time/(1e-3*ps));

    fAnalysisManager->AddNtupleRow(NtupleID);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief 


*/
void RunAction::ReadInput()
{
  // TODO: Clear fW,fX,... before import
  std::ifstream line;
  line.open(fInFileName);

  G4double w,x,y,z,px,py,pz,t;
  
  while (!line.eof()) // TODO: if file[0]!="#"
  {
    line >> w >> x >> y >> z >> px >> py >> pz >> t;
    fW.push_back(w)    ;
    fX.push_back(x)    ; fY.push_back(y)  ; fZ.push_back(z);
    fPx.push_back(px)  ; fPy.push_back(py); fPz.push_back(pz);
    fT.push_back(t)    ;
  }
  
  line.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/**
\brief Define UI commands.

The input file name can be changed by using 
/input/setFileName fileName

The output file name can be changed by using 
/output/setFileName fileName

*/
void RunAction::SetCommands()
{
  fOutMessenger = new G4GenericMessenger(this,"/output/","Manage simulation output");
  fInMessenger = new G4GenericMessenger(this,"/input/","Manage simulation input");

  G4GenericMessenger::Command& setOutFileNameCmd
    = fOutMessenger->DeclareProperty("setFileName",
                                fOutFileName,
                                "Change output file name");
                                
  G4GenericMessenger::Command& setInFileNameCmd
    = fInMessenger->DeclareProperty("setFileName",
                                fInFileName,
                                "Change input file name");
}
