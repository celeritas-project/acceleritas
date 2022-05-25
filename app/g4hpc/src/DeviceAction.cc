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
#include "celeritas/ext/MpiCommunicator.hh"
#include "celeritas/track/TrackInitParams.hh"

G4int DeviceAction::CountDevices() const
{
    G4int num_devices;
    cudaGetDeviceCount(&num_devices);
    return num_devices;
}

void DeviceAction::ActivateDevice() const
{
    // Initialize GPU
    using celeritas::MpiCommunicator;

    MpiCommunicator comm = MpiCommunicator{};
    celeritas::activate_device(celeritas::make_device(comm));
    celeritas::set_cuda_stack_size(32768);
}

void DeviceAction::PropagateTracks(const Transporter& transport_ptr,
                                   const TrackStack&  tracks) const
{
    CELER_ENSURE(transport_ptr);

    ActivateDevice();
    auto result = (*transport_ptr)(tracks);
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
