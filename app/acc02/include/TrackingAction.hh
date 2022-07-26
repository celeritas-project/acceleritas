//----------------------------------*-C++-*----------------------------------//
// Copyright 2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserTrackingAction.hh"

#include "globals.hh"
#include "TrackStack.hh"

#include "LDemoIO.hh"

class G4Track;

class TrackingAction : public G4UserTrackingAction
{
    using size_type = long unsigned int;
    using id_type   = unsigned int;
    using Result    = demo_loop::TransporterResult;

  public:
    TrackingAction()  = default;
    ~TrackingAction() = default;

    void PreUserTrackingAction(const G4Track* track) final;

    void SetEventID(size_type id) { fEventId = id; }
    void LaunchTask();

  private:
    bool IsApplicable(const G4Track& track) const;
    void DoIt(id_type eventId, const G4Track& track);
    void AddTrack(id_type eventId, const G4Track& track);
    void Synchronize();

    inline size_type StackSize() const { return fStack.size(); }

    void PropagateTracks(const TrackStack& stack) const;

    void CreateHit(const Result& hits) const;

  private:
    size_type  fEventId{0};
    TrackStack fStack;
};
