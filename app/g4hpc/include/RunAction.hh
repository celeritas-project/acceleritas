//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserRunAction.hh"
#include "LDemoIO.hh"

class G4Run;
class DeviceManager;

class RunAction : public G4UserRunAction
{
    using Arguments = demo_loop::LDemoArgs;
    using r_pointer = std::unique_ptr<RunAction>;
    using u_pointer = std::unique_ptr<DeviceManager>;

  public:
    RunAction();
    ~RunAction() = default;

    void BeginOfRunAction(const G4Run* run) final;

    static RunAction*     Instance();
    static Arguments      GetArgs() { return gArgs; }
    static DeviceManager* GetDeviceManager() { return fDeviceManager.get(); }

  private:
    static Arguments gArgs;
    static r_pointer gInstance;
    static u_pointer fDeviceManager;
};
