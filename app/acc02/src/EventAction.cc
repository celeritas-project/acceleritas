//----------------------------------*-C++-*----------------------------------//
// Copyright 2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "EventAction.hh"

#include "G4Event.hh"

void EventAction::BeginOfEventAction(const G4Event* event)
{
    // Initialize the tracking action
    fTrackingAction->SetEventID(event->GetEventID());
}

void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
    // Process left-over tracks
    if (Configuration::Instance()->GetOffLoad())
    {
        G4cout << "End of Event Action" << G4endl;
        fTrackingAction->LaunchTask();
    }
}
