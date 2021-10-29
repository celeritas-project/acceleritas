//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "EventAction.hh"

#include "G4Event.hh"
#include "RunAction.hh"
#include "TrackingAction.hh"
#include "DeviceManager.hh"

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    fTrackingAction->SetEventID(evt->GetEventID());
}

void EventAction::EndOfEventAction(const G4Event* /* evt */)
{
    // Process left-over tracks
    RunAction::Instance()->GetDeviceManager()->LaunchTask();
}
