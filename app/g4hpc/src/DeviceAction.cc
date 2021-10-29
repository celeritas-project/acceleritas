//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceAction.cc
//---------------------------------------------------------------------------//
#include "DeviceAction.hh"
#include "RunAction.hh"

#include "LDemoRun.hh"
#include "LDemoParams.hh"
#include "comm/Logger.hh"
#include "comm/Communicator.hh"

G4int DeviceAction::CountDevices() const
{
    G4int num_devices;
    cudaGetDeviceCount(&num_devices);
    return num_devices;
}

void DeviceAction::ActivateDevice() const
{
    using celeritas::Communicator;

    Communicator comm = Communicator{};

    // Initialize GPU
    celeritas::activate_device(celeritas::Device::from_round_robin(comm));

    if (!celeritas::device())
    {
        CELER_LOG(critical) << "CUDA capability is disabled";
    }

    celeritas::set_cuda_stack_size(32768);
}

void DeviceAction::PropagateTracks(const TrackStack& tracks) const
{
    ActivateDevice();

    G4cout << "TODO: pass " << tracks.size() << " tracks to run_gpu" << G4endl;
    auto result = run_gpu(RunAction::GetArgs());

    CreateHit(result);
}

void DeviceAction::CreateHit(const Result& hits) const
{
    G4cout << "TODO: Create hits with size " << hits.edep.size() << G4endl;
}
