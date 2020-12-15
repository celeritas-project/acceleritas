//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeneratorAction.cc
//---------------------------------------------------------------------------//
#include "GeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4HEPEvtInterface.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

GeneratorAction::GeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,1.));
  fParticleGun->SetParticleEnergy(50.*MeV);

  //the primary position of collisions
  fParticleGun->SetParticlePosition( G4ThreeVector( 0.0, 0.0, 0.0 ) ); 

  const char* filename = "input/hepevt.data";
  HEPEvt = new G4HEPEvtInterface(filename);
  useHEPEvt = true;

}

GeneratorAction::~GeneratorAction()
{
  delete HEPEvt;
  delete fParticleGun;
}

void GeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(useHEPEvt) { 
    HEPEvt->GeneratePrimaryVertex(anEvent);
  }
  else {

    if(minEta > maxEta)  G4cout << "Warning: minEta > maxEta" ;
    if(minPhi > maxPhi)  G4cout << "Warning: minPhi > maxPhi" ;

    G4double eta = 0.0;
    G4double phi = 0.0;

    for(G4int np = 0 ; np < nParticle ; np++) {
      
      eta = minEta + (maxEta - minEta)*G4UniformRand();
      phi = minPhi + (maxPhi - minPhi)*G4UniformRand();
      
      G4double theta = 2.*std::atan(std::exp(-eta));
      
      G4ThreeVector direction(std::sin(theta)*std::cos(phi),
                              std::sin(theta)*std::sin(phi), std::cos(theta));
      
      fParticleGun->SetParticleMomentumDirection( direction );
      fParticleGun->GeneratePrimaryVertex( anEvent );
    }
  }

  // Set gun position
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
