//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "TrackingAction.hh"

#include "G4Track.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    if (GetDeviceManager()->IsApplicable(*track))
    {
        GetDeviceManager()->DoIt(fEventId, *track);
        (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);
    }
}
