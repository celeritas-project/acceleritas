//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4Timer.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include <iomanip>

EventAction::EventAction()
  : G4UserEventAction(),
    totalEventCPUTime(0.0)
{
  eventTimer = new G4Timer;
}

EventAction::~EventAction()
{
  delete eventTimer;
}

void EventAction::BeginOfEventAction(const G4Event*)
{  
  eventTimer->Start();
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  eventTimer->Stop();

  double eventCpuTime = eventTimer->GetUserElapsed() 
    + eventTimer->GetSystemElapsed();
  totalEventCPUTime += eventCpuTime;

  G4int precision_t = G4cout.precision(3);
  std::ios::fmtflags flags_t = G4cout.flags();
  G4cout.setf(std::ios::fixed,std::ios::floatfield); 
  G4cout << "TimeEvent> "<< evt->GetEventID()+1 << " "
         << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()+1 << " "
         << eventTimer->GetRealElapsed() << " " << eventCpuTime << " "
         << totalEventCPUTime << G4endl;
  G4cout.setf(flags_t);
  G4cout.precision(precision_t);

}  
