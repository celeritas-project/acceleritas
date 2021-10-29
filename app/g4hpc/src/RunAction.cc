//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"
#include "Configuration.hh"
#include "DeviceManager.hh"

#include "G4Run.hh"

RunAction::r_pointer RunAction::gInstance;
RunAction::u_pointer RunAction::fDeviceManager;
demo_loop::LDemoArgs RunAction::gArgs;

RunAction* RunAction::Instance()
{
    if (gInstance.get() == nullptr)
        gInstance = r_pointer(new RunAction);
    return gInstance.release();
}

RunAction::RunAction() : G4UserRunAction()
{
    fDeviceManager = u_pointer(new DeviceManager());
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

    Configuration* config   = Configuration::Instance();
    gArgs.geometry_filename = config->GetGeometryFile();
    gArgs.physics_filename  = config->GetPhysicsFile();
    gArgs.hepmc3_filename   = config->GetHepMC3File();
    gArgs.seed              = config->GetSeed();
    gArgs.max_num_tracks    = config->GetMaxNumTracks();
    gArgs.max_steps         = config->GetMaxSteps();
    gArgs.storage_factor    = config->GetStorageFactor();

    // Initialize the user task manager
    long int nthreads = config->GetNumUserThreads();
    fDeviceManager->InitializeTaskManager(nthreads);
    fDeviceManager->TaskRunManagerInfo();
}
