//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"

#include "G4Run.hh"
#include "G4Timer.hh"

RunAction::RunAction()
 : G4UserRunAction()
{ 
  fTimer = new G4Timer;
}

RunAction::~RunAction()
{
  delete fTimer;
}

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  fTimer->Start();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  fTimer->Stop();

  G4int oldPrecision = G4cout.precision(3);
  std::ios::fmtflags oldFlags = G4cout.flags();
  G4cout.setf(std::ios::fixed,std::ios::floatfield); 
  G4cout << "TimeReport> Time report complete in ";
  if ( fTimer->IsValid() ) {
    G4cout << fTimer->GetRealElapsed();
  } else {
    G4cout << "UNDEFINED";
  }
  G4cout << " seconds" << G4endl;
  G4cout.setf(oldFlags);
  G4cout.precision(oldPrecision);
}
