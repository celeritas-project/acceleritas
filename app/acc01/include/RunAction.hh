//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
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
    using u_pointer = std::unique_ptr<DeviceManager>;

  public:
    RunAction();
    ~RunAction() = default;

    void BeginOfRunAction(const G4Run* run) final;
    void EndOfRunAction(const G4Run* run) final;

    static RunAction*     Instance();
    static Arguments      GetArgs() { return gArgs; }
    static DeviceManager* GetDeviceManager() { return fDeviceManager.get(); }

  private:
    static RunAction* gInstance;
    static Arguments  gArgs;
    static u_pointer  fDeviceManager;
};
