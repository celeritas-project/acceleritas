//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DeviceManager.cc
//---------------------------------------------------------------------------//
#include "DeviceManager.hh"
#include "Configuration.hh"
#include "RunAction.hh"

#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4SystemOfUnits.hh"

#include "corecel/device_runtime_api.h"
#include "corecel/Assert.hh"
#include "celeritas/phys/PDGNumber.hh"
#include "celeritas/phys/ParticleParams.hh"

namespace
{
void Synchronize()
{
    CELER_CUDA_CALL(cudaDeviceSynchronize());
}
} // namespace

TrackStack               DeviceManager::fStack;
DeviceManager::a_pointer DeviceManager::fAction;
DeviceManager::m_pointer DeviceManager::fManager;
DeviceManager::t_pointer DeviceManager::fTransport;

DeviceManager::DeviceManager()
{
    fAction  = a_pointer(new DeviceAction());
    fManager = m_pointer(new TaskRunManager());
}

void DeviceManager::DeviceTask(const TrackStack& tracks)
{
    fAction.get()->PropagateTracks(fTransport, tracks);
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

    // Create transporter when offloading is enabled
    if (Configuration::Instance()->GetOffLoad())
    {
        fAction.get()->ActivateDevice();
        demo_loop::LDemoArgs run_args = RunAction::GetArgs();
        fTransport                    = demo_loop::build_transporter(run_args);
        CELER_ENSURE(fTransport);
    }
}

bool DeviceManager::IsApplicable(const G4Track& track) const
{
    // Condition for triggering the device task
    G4ParticleDefinition* ptype = track.GetDefinition();
    return (ptype == G4Gamma::Gamma() || ptype == G4Electron::Electron()
            || ptype == G4Positron::Positron());
}

void DeviceManager::DoIt(id_type eventId, const G4Track& track)
{
    // Store this track to the stack
    AddTrack(eventId, track);

    if (StackSize() == Configuration::Instance()->GetChunkTracks())
    {
        LaunchTask();
    }
}

void DeviceManager::AddTrack(id_type eventId, const G4Track& track)
{
    using namespace celeritas;

    celeritas::PDGNumber  pdg{track.GetDefinition()->GetPDGEncoding()};
    celeritas::ParticleId pid{fTransport->params().particle()->find(pdg)};

    G4ThreeVector pos  = track.GetPosition();
    G4ThreeVector dir  = track.GetMomentumDirection();
    G4double      time = track.GetGlobalTime();
    unsigned int  tid  = track.GetTrackID();

    fStack.push_back({pid,
                      units::MevEnergy{track.GetKineticEnergy()},
                      {pos.x() / cm, pos.y() / cm, pos.z() / cm},
                      {dir.x(), dir.y(), dir.z()},
                      time,
                      EventId{eventId},
                      TrackId{tid}});
}

void DeviceManager::LaunchTask()
{
    TaskGroup<void> device_tasks(Synchronize, fManager->GetThreadPool());

    // Submit work to task-groups
    device_tasks.exec(DeviceTask, fStack);
    device_tasks.join();

    // Clear tracks
    fStack.clear();
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
