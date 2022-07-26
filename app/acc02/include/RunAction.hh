//----------------------------------*-C++-*----------------------------------//
// Copyright 2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserRunAction.hh"
#include "LDemoIO.hh"
#include "Transporter.hh"
#include "corecel/sys/Device.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
    using Arguments = demo_loop::LDemoArgs;
    using t_pointer = std::unique_ptr<demo_loop::TransporterBase>;

  public:
    RunAction() : G4UserRunAction() {}
    ~RunAction() = default;

    void BeginOfRunAction(const G4Run* run) final;
    void EndOfRunAction(const G4Run* run) final;

    static RunAction*                  Instance();
    static Arguments                   GetArgs() { return gArgs; }
    static demo_loop::TransporterBase* GetTransporter()
    {
        CELER_ENSURE(fTransport);
        return fTransport.get();
    }

    inline void  ResetTransport() { fTransport.reset(); }
    static G4int CountDevices() { return celeritas::Device::num_devices(); }

  private:
    void ActivateDevice() const;

  private:
    static RunAction* gInstance;
    static Arguments  gArgs;
    static t_pointer  fTransport;
};
