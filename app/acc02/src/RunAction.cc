//----------------------------------*-C++-*----------------------------------//
// Copyright 2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"
#include "Configuration.hh"

#include "G4Run.hh"
#include "G4Threading.hh"

#include "celeritas/ext/MpiCommunicator.hh"
#include "celeritas/track/TrackInitParams.hh"

RunAction*           RunAction::gInstance = nullptr;
demo_loop::LDemoArgs RunAction::gArgs;
RunAction::t_pointer RunAction::fTransport = nullptr;

RunAction* RunAction::Instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new RunAction;
    }
    return gInstance;
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

    if (IsMaster() && run != nullptr)
    {
        Configuration* config = Configuration::Instance();

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

        gArgs.geant_options.msc = config->GetEnableMsc()
                                      ? celeritas::MscModelSelection::urban
                                      : celeritas::MscModelSelection::none;

        if (config->GetOffLoad())
        {
            G4int threadID = G4Threading::G4GetThreadId();
            G4int deviceID = threadID % RunAction::CountDevices();
            G4cout << "Number of Devices = " << CountDevices() << G4endl;
            G4cout << "ActivateDevice: threadID = " << threadID
                   << " deviceID = " << deviceID << G4endl;
            ActivateDevice();
            celeritas::set_cuda_stack_size(32768);

            demo_loop::LDemoArgs run_args = GetArgs();
            fTransport = demo_loop::build_transporter(run_args);
            CELER_ENSURE(fTransport);
        }
    }
}

void RunAction::EndOfRunAction(const G4Run* /* run */)
{
    G4cout << "End of Run Action" << G4endl;
    ResetTransport();
}

void RunAction::ActivateDevice() const
{
    CELER_CUDA_CALL(cudaDeviceReset());

    // Initialize GPU
    using celeritas::MpiCommunicator;

    MpiCommunicator comm = MpiCommunicator{};
    celeritas::activate_device(celeritas::make_device(comm));

    celeritas::set_cuda_stack_size(32768);
}
