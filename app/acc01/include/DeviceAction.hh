//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
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
    using Result      = demo_loop::TransporterResult;
    using Transporter = std::unique_ptr<demo_loop::TransporterBase>;
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
