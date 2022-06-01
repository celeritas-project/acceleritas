//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "TrackingAction.hh"

#include "G4AutoLock.hh"
#include "G4Track.hh"

namespace
{
G4Mutex doit_mutex = G4MUTEX_INITIALIZER;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    if (Configuration::Instance()->GetOffLoad()
        && GetDeviceManager()->IsApplicable(*track))
    {
        G4AutoLock lock(&doit_mutex);
        GetDeviceManager()->DoIt(fEventId, *track);
        (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);
    }
}
