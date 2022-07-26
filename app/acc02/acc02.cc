//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file g4hpc.cc
//---------------------------------------------------------------------------//
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "G4RunManagerFactory.hh"
#include "G4Threading.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 5)
    {
        G4cout << "Usage: g4hpc -m run.mac [-t nthreads]" << G4endl;
        return -1;
    }

    G4String macro;
    G4int    nthreads = G4Threading::G4GetNumberOfCores();

    for (G4int i = 1; i < argc; i = i + 2)
    {
        if (G4String(argv[i]) == "-m")
        {
            macro = argv[i + 1];
        }
        if (G4String(argv[i]) == "-t")
        {
            nthreads = G4UIcommand::ConvertToInt(argv[i + 1]);
        }
    }

    // Set the random seed
    CLHEP::HepRandom::setTheSeed(1245214UL);

    auto* runmanager
        = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Tasking);

    // Set the number of threads
    if (runmanager)
    {
        G4MTRunManager* mt = dynamic_cast<G4MTRunManager*>(runmanager);

        if (mt)
        {
            runmanager->SetNumberOfThreads(nthreads);
            G4cout << "\ng4hpc: event loop running in MT with " << nthreads
                   << " threads out of " << G4Threading::G4GetNumberOfCores()
                   << " available cores\n"
                   << G4endl;
        }
    }

    // Set user actions and initialize run
    auto detector = std::make_unique<DetectorConstruction>();
    runmanager->SetUserInitialization(detector.release());

    auto physics_list = std::make_unique<FTFP_BERT>();
    runmanager->SetUserInitialization(physics_list.release());

    auto action_init = std::make_unique<ActionInitialization>();
    runmanager->SetUserInitialization(action_init.release());

    // Set the user interface
    G4UImanager* UI = G4UImanager::GetUIpointer();

    G4String command = "/control/execute ";
    UI->ApplyCommand(command + macro);

    runmanager->Initialize();

    //@termination
    delete runmanager;

    return 0;
}
