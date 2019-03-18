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
// $Id: PhysicsListSimple.cc 107183 2017-11-03 14:57:23Z gcosmo $

#include "PhysicsListSimple.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
// #include "G4ParticleTable.hh"

// *** Processes and models

// gamma

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

// e- and e+

#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

// msc models
#include "G4UrbanMscModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
//

// particles

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

//
#include "G4PhysicsListHelper.hh"
// #include "G4BuilderType.hh"
// #include "G4EmModelActivator.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(PhysicsListSimple);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListSimple::PhysicsListSimple()
  : G4VPhysicsConstructor("PhysicsListSimple")
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(1);
  param->SetMinEnergy(100*eV);
  param->SetMaxEnergy(1*TeV);
  param->SetLowestElectronEnergy(100*eV);
  param->SetNumberOfBinsPerDecade(20);
  param->SetMscRangeFactor(0.02);
  param->SetMscStepLimitType(fUseDistanceToBoundary);
  // param->SetMuHadLateralDisplacement(true);
  // param->SetLateralDisplacementAlg96(false);
  // param->SetFluo(true);
  // param->SetPIXEElectronCrossSectionModel("Penelope");
  // SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListSimple::~PhysicsListSimple()
{
  delete fGamma;
  delete fElectron;
  delete fPositron;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListSimple::ConstructParticle()
{
  // gamma
  fGamma=G4Gamma::Gamma();

  // leptons
  fElectron=G4Electron::Electron();
  fPositron=G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

  // baryons
  G4Proton::Proton();
  G4AntiProton::AntiProton();

  // ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListSimple::ConstructProcess()
{
  // G4cout << "### " << GetPhysicsName() << " Construct Processes " << G4endl;
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // high energy limit for e+- scattering models
  G4double highEnergyLimit = 100*MeV;

  //Applicability range for Penelope models
  //for higher energies, the Standard models are used
  G4double PenelopeHighEnergyLimit = 1.0*GeV;

  // Add Penelope EM Processes
  // G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  // G4ParticleDefinition* particle;

  // processes for gamma
  // particle = table->FindParticle("gamma");
  // particle = G4Gamma::Gamma();
  //Gamma conversion
  G4GammaConversion* theGammaConversion = new G4GammaConversion();
  G4PenelopeGammaConversionModel* theGCPenelopeModel = new G4PenelopeGammaConversionModel();
  theGammaConversion->SetEmModel(theGCPenelopeModel);
  // ph->RegisterProcess(theGammaConversion, particle);
  // ph->RegisterProcess(theGammaConversion, G4Gamma::Gamma());
  ph->RegisterProcess(theGammaConversion, fGamma);

  // processes for e-
  // particle = table->FindParticle("e-");
  // particle = G4Electron::Electron();
  // multiple scattering
  G4eMultipleScattering* msc = new G4eMultipleScattering;
  G4UrbanMscModel* msc1 = new G4UrbanMscModel();
  G4WentzelVIModel* msc2 = new G4WentzelVIModel();
  msc1->SetHighEnergyLimit(highEnergyLimit);
  msc2->SetLowEnergyLimit(highEnergyLimit);
  msc->SetEmModel(msc1);
  msc->SetEmModel(msc2);

  G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();
  G4CoulombScattering* ss = new G4CoulombScattering();
  ss->SetEmModel(ssm);
  ss->SetMinKinEnergy(highEnergyLimit);
  ssm->SetLowEnergyLimit(highEnergyLimit);
  ssm->SetActivationLowEnergyLimit(highEnergyLimit);

  //Bremsstrahlung
  G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
  G4PenelopeBremsstrahlungModel* theBremPenelope = new G4PenelopeBremsstrahlungModel();
  theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
  eBrem->SetEmModel(theBremPenelope);

  // register processes
  // ph->RegisterProcess(msc, particle);
  // ph->RegisterProcess(eBrem, particle);
  // ph->RegisterProcess(ss, particle);
  // ph->RegisterProcess(msc, G4Electron::Electron());
  // ph->RegisterProcess(eBrem, G4Electron::Electron());
  // ph->RegisterProcess(ss, G4Electron::Electron());
  ph->RegisterProcess(msc, fElectron);
  ph->RegisterProcess(eBrem, fElectron);
  ph->RegisterProcess(ss, fElectron);

  // processes for e+
  // particle = table->FindParticle("e+");
  // particle = G4Positron::Positron();
  // register processes
  // ph->RegisterProcess(msc, particle);
  // ph->RegisterProcess(ss, particle);
  // ph->RegisterProcess(msc, G4Positron::Positron());
  // ph->RegisterProcess(ss, G4Positron::Positron());
  ph->RegisterProcess(msc, fPositron);
  ph->RegisterProcess(ss, fPositron);

  // G4EmModelActivator mact(GetPhysicsName());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
