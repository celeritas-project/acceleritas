//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceManager.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"

#include "PTL/TaskGroup.hh"
#include "PTL/ThreadData.hh"
#include "PTL/ThreadPool.hh"
#include "PTL/TaskRunManager.hh"

#include "TrackStack.hh"
#include "DeviceAction.hh"

#include "cuda_runtime.h"

class G4Track;

using namespace PTL;

class DeviceManager
{
  public:
    using id_type   = unsigned int;
    using size_type = long unsigned int;
    using a_pointer = std::shared_ptr<DeviceAction>;
    using m_pointer = std::shared_ptr<TaskRunManager>;

  public:
    DeviceManager();
    ~DeviceManager() = default;

    inline static thread_local TaskRunManager* GetTaskManager()
    {
        return fManager.get();
    }

    void InitializeTaskManager(uintmax_t nthreads);
    void TaskRunManagerInfo() const;
    void LaunchTask();

    virtual bool IsApplicable(const G4Track& track) const;
    virtual void DoIt(id_type eventId, const G4Track& track);

  private:
    inline static void Synchronize() { cudaDeviceSynchronize(); }
    static void        DeviceTask(const TrackStack& tracks);

    void      AddTrack(id_type eventId, const G4Track& track);
    size_type StackSize() const { return fStack.size(); }

  private:
    static thread_local TrackStack fStack;
    static thread_local a_pointer  fAction;
    static thread_local m_pointer  fManager;
};
