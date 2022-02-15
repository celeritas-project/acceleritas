//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.cc
//---------------------------------------------------------------------------//
#include "ActionInitialization.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "GeneratorAction.hh"

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    TrackingAction* trackingAction = new TrackingAction;
    SetUserAction(trackingAction);
    SetUserAction(new EventAction(trackingAction));
    SetUserAction(new GeneratorAction);
}
