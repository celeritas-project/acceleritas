//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "TrackingAction.hh"
#include "RunAction.hh"
#include "DeviceManager.hh"
#include "Configuration.hh"

#include "G4Track.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    G4int pid = track->GetDefinition()->GetPDGEncoding();

    // If the particle is either photon or electron/positron
    if (pid == 22 || std::abs(pid) == 11)
    {
        DeviceManager* fDeviceManager
            = RunAction::Instance()->GetDeviceManager();

        // Store the track into TrackStack and kill it
        fDeviceManager->AddTrack(fEventId, pid, *track);
        ++fNumberOfTrcks;
        (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);

        // If the size of the EM stack is fChunkElectrons
        if (fNumberOfTrcks == Configuration::Instance()->GetChunkTracks())
        {
            // Lauch a kernel with this chuck of EM particles
            fDeviceManager->LaunchTask();
            fNumberOfTrcks = 0;
        }
    }
}
