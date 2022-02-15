//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeneratorAction.cc
//---------------------------------------------------------------------------//
#include "GeneratorAction.hh"
#include "Configuration.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4HEPEvtInterface.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

GeneratorAction::GeneratorAction() : G4VUserPrimaryGeneratorAction()
{
    fParticleGun = new G4ParticleGun(1);

    // default particle kinematic
    G4ParticleDefinition* particleDefinition
        = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 1.));
    fParticleGun->SetParticleEnergy(50. * GeV);

    // the primary position of collisions
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));

    const char* filename = "input/hepevt.data";
    fHEPEvt              = new G4HEPEvtInterface(filename);
}

void GeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (Configuration::Instance()->GetUseHEPEvtGenerator())
    {
        fHEPEvt->GeneratePrimaryVertex(anEvent);
    }
    else
    {
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
