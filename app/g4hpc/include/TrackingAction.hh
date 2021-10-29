//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserTrackingAction.hh"

class G4Track;

class TrackingAction : public G4UserTrackingAction
{
    using size_type = long unsigned int;

  public:
    TrackingAction() {}
    ~TrackingAction() = default;

    void PreUserTrackingAction(const G4Track* track) final;
    void SetEventID(size_type id) { fEventId = id; }

  private:
    size_type fEventId{0};
    size_type fNumberOfTrcks{0};
};
