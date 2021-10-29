//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceManager.cc
//---------------------------------------------------------------------------//
#include "DeviceManager.hh"

#include "G4Track.hh"

thread_local DeviceManager::a_pointer DeviceManager::fAction;
thread_local DeviceManager::m_pointer DeviceManager::fManager;

DeviceManager::DeviceManager()
{
    fAction  = a_pointer(new DeviceAction());
    fManager = m_pointer(new TaskRunManager());
}

void DeviceManager::DeviceTask(const TrackStack& tracks)
{
    fAction.get()->PropagateTracks(tracks);
}

void DeviceManager::InitializeTaskManager(uintmax_t nthreads)
{
    // Initialize this task run manager
    if (!fManager->IsInitialized())
    {
        fManager->Initialize(nthreads);
    }

    // Create and initialize thread data
    ThreadData*& thread_data = ThreadData::GetInstance();
    if (!thread_data)
        thread_data = new ThreadData(fManager->GetThreadPool());
    thread_data->is_main     = false;
    thread_data->within_task = false;
}

void DeviceManager::AddTrack(id_type eventId, id_type pid, const G4Track& track)
{
    using namespace celeritas;

    G4ThreeVector pos = track.GetPosition();
    G4ThreeVector dir = track.GetMomentumDirection();
    unsigned int  tid = track.GetTrackID();

    fEmStack.push_back({ParticleId{pid},
                        units::MevEnergy{track.GetKineticEnergy()},
                        {pos.x(), pos.y(), pos.z()},
                        {dir.x(), dir.y(), dir.z()},
                        EventId{eventId},
                        TrackId{tid}});
}

void DeviceManager::LaunchTask()
{
    TaskGroup<void> device_tasks(Synchronize, fManager->GetThreadPool());

    // Submit work to task-groups
    device_tasks.exec(DeviceTask, fEmStack);
    device_tasks.join();

    // Clear tracks
    fEmStack.clear();
}

void DeviceManager::TaskRunManagerInfo() const
{
    TaskRunManager* rm          = TaskRunManager::GetMasterRunManager();
    ThreadPool*     thread_pool = (rm) ? rm->GetThreadPool() : nullptr;
    G4cout << " Master ThreadPool size = " << thread_pool->size() << G4endl;

    intmax_t m_workers = (rm) ? rm->GetNumberOfThreads() + 1
                              : (2 * std::thread::hardware_concurrency()) + 1;
    G4cout << " Number of Workers  = " << m_workers << G4endl;
    G4cout << " Number of GPUs = " << fAction.get()->CountDevices() << G4endl;
    G4cout << " Number of User workers = " << fManager->GetNumberOfThreads()
           << G4endl;
}
