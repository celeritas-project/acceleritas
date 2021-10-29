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
    using a_pointer = std::shared_ptr<DeviceAction>;
    using m_pointer = std::shared_ptr<TaskRunManager>;

  public:
    DeviceManager();
    ~DeviceManager() = default;

    inline static thread_local TaskRunManager* GetTaskManager()
    {
        return fManager.get();
    }

    inline static void Synchronize() { cudaDeviceSynchronize(); }
    static void        DeviceTask(const TrackStack& tracks);

    void InitializeTaskManager(uintmax_t nthreads);
    void AddTrack(id_type eventId, id_type pid, const G4Track& track);
    void LaunchTask();

    G4int StackSize() const { return fEmStack.size(); }

    void TaskRunManagerInfo() const;

  private:
    TrackStack                    fEmStack;
    static thread_local a_pointer fAction;
    static thread_local m_pointer fManager;
};
