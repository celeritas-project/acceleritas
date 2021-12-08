//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "TrackStack.hh"

#include "LDemoIO.hh"
#include "Transporter.hh"

class DeviceAction
{
    using Result      = celeritas::TransporterResult;
    using Transporter = std::unique_ptr<celeritas::TransporterBase>;
    using LDemoArgs   = demo_loop::LDemoArgs;

  public:
    DeviceAction() {}
    ~DeviceAction() = default;

    G4int CountDevices() const;
    void  ActivateDevice() const;
    void  PropagateTracks(const Transporter& transport_ptr,
                          const TrackStack&  stack) const;

    void CreateHit(const Result& hits) const;
};
