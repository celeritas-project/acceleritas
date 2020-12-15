//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4UserEventAction.hh"

class G4Timer;

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();
  
  virtual void BeginOfEventAction(const G4Event* evt);
  virtual void EndOfEventAction(const G4Event* evt);
  
private:
  G4double totalEventCPUTime;
  G4Timer* eventTimer;
};
