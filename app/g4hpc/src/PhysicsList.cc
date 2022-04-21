//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsList.cc
//---------------------------------------------------------------------------//
#include <iomanip>

#include "globals.hh"
#include "G4ios.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"

#include "PhysicsList.hh"
#include "EmStandardNoMsc.hh"

//---------------------------------------------------------------------------//
/*!
 * This class is a copy of FTFP_BERT.cc of Geant4 11.0 and modified for g4hpc.
 */
PhysicsList::PhysicsList(G4int ver, G4int msc_option)
{
    if (ver > 0)
    {
        G4cout << "<<< Physics List: FTFP_BERT with g4hpc extentions" << G4endl;
        G4cout << G4endl;
    }
    defaultCutValue = 0.7 * CLHEP::mm;
    SetVerboseLevel(ver);

    // EM Physics with and without e-/e+ MSC processes
    if (msc_option == 0)
    {
        RegisterPhysics(new EmStandardNoMsc(ver));
        G4cout << "<<< PhysicsList using EmStandardNoMsc" << G4endl;
        G4cout << G4endl;
    }
    else
    {
        RegisterPhysics(new G4EmStandardPhysics(ver));
        G4cout << "<<< PhysicsList using G4EmStandardPhysics" << G4endl;
        G4cout << G4endl;
    }
    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics(ver));

    // Decays
    RegisterPhysics(new G4DecayPhysics(ver));

    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysics(ver));

    // Hadron Physics
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT(ver));

    // Stopping Physics
    RegisterPhysics(new G4StoppingPhysics(ver));

    // Ion Physics
    RegisterPhysics(new G4IonPhysics(ver));

    // Neutron tracking cut
    RegisterPhysics(new G4NeutronTrackingCut(ver));
}
