//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceAction.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "DeviceAction.hh"
#include "RunAction.hh"

#include "LDemoIO.hh"
#include "Transporter.hh"
#include "sim/TrackInitParams.hh"

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

void DeviceAction::PropagateTracks(const Transporter& transport_ptr,
                                   const TrackStack&  tracks) const
{
    CELER_ENSURE(transport_ptr);

    ActivateDevice();

    demo_loop::LDemoArgs run_args = RunAction::GetArgs();

    using celeritas::TrackInitParams;

    TrackInitParams::Input input_data;
    input_data.primaries = tracks;
    input_data.capacity  = run_args.initializer_capacity;

    std::shared_ptr<celeritas::TrackInitParams> primaries
        = std::make_shared<TrackInitParams>(std::move(input_data));

    auto result = (*transport_ptr)(*primaries);
    CreateHit(result);
}

void DeviceAction::CreateHit(const Result& hits) const
{
    G4cout << "TODO: Create hits with size " << hits.edep.size() << G4endl;

    if (Configuration::Instance()->GetEnableDiagnostics())
    {
        G4double edep_total = 0;
        for (auto edep : hits.edep)
        {
            edep_total += edep;
        }
        G4cout << "total deposited energy  " << edep_total << G4endl;
        G4cout << "total time (offloading) " << hits.time.total << G4endl;
    }
}
