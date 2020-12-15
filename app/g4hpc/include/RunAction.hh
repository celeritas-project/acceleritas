//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4UserRunAction.hh"

class G4Run;
class G4Timer;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();
  
  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

private:
  G4Timer* fTimer;
};
