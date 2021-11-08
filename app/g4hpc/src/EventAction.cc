//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "EventAction.hh"

#include "G4Event.hh"
#include "TrackingAction.hh"

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    fTrackingAction->SetEventID(evt->GetEventID());
}
