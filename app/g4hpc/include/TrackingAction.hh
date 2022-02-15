//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "RunAction.hh"
#include "DeviceManager.hh"

#include "G4UserTrackingAction.hh"

class G4Track;

class TrackingAction : public G4UserTrackingAction
{
    using size_type = long unsigned int;

  public:
    TrackingAction()  = default;
    ~TrackingAction() = default;

    void PreUserTrackingAction(const G4Track* track) final;
    void SetEventID(size_type id) { fEventId = id; }

  private:
    inline static DeviceManager* GetDeviceManager()
    {
        return RunAction::Instance()->GetDeviceManager();
    }

  private:
    size_type fEventId{0};
};
