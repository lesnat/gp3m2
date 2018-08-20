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
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4TrackingManager.hh"
#include "G4DynamicParticle.hh"

#include "TrackingAction.hh"
#include "G4Track.hh"

#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief 

*/
TrackingAction::TrackingAction()
: G4UserTrackingAction(),
  fGamma(0),
  fPositron(0),
  fBiasingFactorGamma(10),
  fBiasingFactorPositron(1)
{
  // get particles definition
  fGamma    = G4Gamma::Gamma();
  fPositron = G4Positron::Positron();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief

*/
TrackingAction::~TrackingAction()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
\brief

*/
void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  G4TrackingManager* trackingManager = new G4TrackingManager();  
  
  const G4DynamicParticle* dynamicParticle = aTrack->GetDynamicParticle();
  
  G4int biasingFactor=1;

  if (dynamicParticle->GetDefinition() == fGamma)
  { 
    biasingFactor = fBiasingFactorGamma;
  }
  else if (dynamicParticle->GetDefinition() == fPositron)
  {
    biasingFactor = fBiasingFactorPositron;
  }
/*  else
  {
    break;
  }
*/
  
  G4double weight = aTrack->GetWeight();
  
  G4DynamicParticle* newDynamicParticle
    = new G4DynamicParticle(dynamicParticle->GetDefinition(),dynamicParticle->GetMomentum());
  
  for (int i=1 ; i<biasingFactor ; i++)
  {
    G4Track* biasedTrack 
      = new G4Track(newDynamicParticle, aTrack->GetGlobalTime(), aTrack->GetPosition());
    biasedTrack->SetWeight(weight/biasingFactor);
    G4cout << weight << " : " << biasedTrack->GetWeight() << G4endl;
    trackingManager->ProcessOneTrack(biasedTrack);
  }
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
