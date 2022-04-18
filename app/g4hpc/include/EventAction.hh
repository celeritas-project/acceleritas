//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "CalorimeterHit.hh"

#include "G4UserEventAction.hh"

class TrackingAction;

class EventAction : public G4UserEventAction
{
  public:
    explicit EventAction(TrackingAction* action) : fTrackingAction(action) {}
    ~EventAction() = default;

    void BeginOfEventAction(const G4Event* eventt) final;
    void EndOfEventAction(const G4Event* event) final;

  private:
    G4int           fcalorHCID      = -1;
    TrackingAction* fTrackingAction = nullptr;

    // Helper function
    CalorimeterHitsCollection*
    GetHitsCollection(G4int hcID, const G4Event* event) const;
};
