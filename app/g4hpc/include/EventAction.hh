//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserEventAction.hh"

class TrackingAction;

class EventAction : public G4UserEventAction
{
  public:
    explicit EventAction(TrackingAction* action) : fTrackingAction(action) {}
    ~EventAction() = default;

    void BeginOfEventAction(const G4Event* evt) final;
    void EndOfEventAction(const G4Event* evt) final;

  private:
    TrackingAction* fTrackingAction = nullptr;
};
