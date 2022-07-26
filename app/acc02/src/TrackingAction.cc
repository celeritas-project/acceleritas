//----------------------------------*-C++-*----------------------------------//
// Copyright 2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "TrackingAction.hh"
#include "RunAction.hh"

#include "G4AutoLock.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

#include "corecel/device_runtime_api.h"
#include "corecel/Assert.hh"
#include "celeritas/phys/PDGNumber.hh"
#include "celeritas/phys/ParticleParams.hh"

namespace
{
G4Mutex doit_mutex = G4MUTEX_INITIALIZER;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    if (Configuration::Instance()->GetOffLoad() && IsApplicable(*track))
    {
        DoIt(fEventId, *track);
        (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);
    }
}

bool TrackingAction::IsApplicable(const G4Track& track) const
{
    // Condition for triggering the device task
    G4ParticleDefinition* ptype = track.GetDefinition();
    return (ptype == G4Gamma::Gamma() || ptype == G4Electron::Electron()
            || ptype == G4Positron::Positron());
}

void TrackingAction::DoIt(id_type eventId, const G4Track& track)
{
    // Store this track to the stack
    AddTrack(eventId, track);

    if (StackSize() == Configuration::Instance()->GetChunkTracks())
    {
        LaunchTask();
    }
}

void TrackingAction::Synchronize()
{
    CELER_CUDA_CALL(cudaDeviceSynchronize());
}

void TrackingAction::AddTrack(id_type eventId, const G4Track& track)
{
    using namespace celeritas;

    auto                  fTransport = RunAction::GetTransporter();
    celeritas::PDGNumber  pdg{track.GetDefinition()->GetPDGEncoding()};
    celeritas::ParticleId pid{fTransport->params().particle()->find(pdg)};

    G4ThreeVector pos  = track.GetPosition();
    G4ThreeVector dir  = track.GetMomentumDirection();
    G4double      time = track.GetGlobalTime();
    unsigned int  tid  = track.GetTrackID();

    fStack.push_back({pid,
                      units::MevEnergy{track.GetKineticEnergy()},
                      {pos.x() / cm, pos.y() / cm, pos.z() / cm},
                      {dir.x(), dir.y(), dir.z()},
                      time,
                      EventId{eventId},
                      TrackId{tid}});
}

void TrackingAction::LaunchTask()
{
    G4cout << "LauchTask" << G4endl;
    G4AutoLock lock(&doit_mutex);
    PropagateTracks(fStack);
    // Clear tracks
    fStack.clear();
}

void TrackingAction::PropagateTracks(const TrackStack& tracks) const
{
    auto transport_ptr = RunAction::GetTransporter();
    CELER_ENSURE(transport_ptr);
    auto result = (*transport_ptr)(tracks);
    CreateHit(result);
}

void TrackingAction::CreateHit(const Result& hits) const
{
    G4cout << "TODO: Create hits with size " << hits.edep.size() << G4endl;

    if (Configuration::Instance()->GetEnableDiagnostics())
    {
        G4double edep_total = 0;
        for (auto edep : hits.edep)
        {
            edep_total += edep;
        }
        G4cout << "Total deposited energy  " << edep_total << G4endl;
        G4cout << "Total time (offloading) " << hits.time.total << G4endl;
    }
}
