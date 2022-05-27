//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"
#include "Configuration.hh"
#include "DeviceManager.hh"

#include "G4AnalysisManager.hh"
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

    Configuration* config = Configuration::Instance();

    if (IsMaster() && run != nullptr)
    {
        gArgs.geometry_filename      = config->GetGeometryFile();
        gArgs.physics_filename       = config->GetPhysicsFile();
        gArgs.hepmc3_filename        = config->GetHepMC3File();
        gArgs.seed                   = config->GetSeed();
        gArgs.max_num_tracks         = config->GetMaxNumTracks();
        gArgs.max_steps              = config->GetMaxSteps();
        gArgs.initializer_capacity   = config->GetCapacity();
        gArgs.secondary_stack_factor = config->GetSecondaryStackFactor();
        gArgs.use_device             = config->GetUseDevice();
        gArgs.enable_diagnostics     = config->GetEnableDiagnostics();
        gArgs.sync                   = config->GetSync();
        gArgs.enable_msc             = config->GetEnableMsc();

        // Initialize the user task manager
        long int nthreads = config->GetNumUserThreads();
        fDeviceManager->InitializeTaskManager(nthreads);
        fDeviceManager->TaskRunManagerInfo();
    }

    if (config->GetEnableAnalysis())
    {
        // Do analysis
        auto analysis_manager = G4AnalysisManager::Instance();
        analysis_manager->OpenFile("g4hpc.root");
        analysis_manager->CreateH1("Edep", "Edep in Ecal", 100, 0., 100000);
    }
}

void RunAction::EndOfRunAction(const G4Run* /* run */)
{
    Configuration* config = Configuration::Instance();

    if (IsMaster())
    {
        // Process left-over tracks and delete the transport ptr
        if (config->GetOffLoad())
        {
            fDeviceManager->LaunchTask();
            fDeviceManager->ResetTransport();
        }
    }

    if (config->GetEnableAnalysis())
    {
        auto analysis_manager = G4AnalysisManager::Instance();
        analysis_manager->Write();
        analysis_manager->CloseFile();
    }
}
