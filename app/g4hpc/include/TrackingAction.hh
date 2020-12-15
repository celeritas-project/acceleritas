//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4UserTrackingAction.hh"

#include <vector>
#include "physics/base/Primary.hh"

class G4Track;
class G4GenericMessenger;

using namespace celeritas;

class TrackingAction : public G4UserTrackingAction
{
  using TrackStack = std::vector<Primary>;

public:

  TrackingAction();
  virtual ~TrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track* track);
  
  void SetChunkElectrons(G4int val) { fChunkElectrons = val; }
  G4int GetChunkElectrons() const { return fChunkElectrons; }

  void SetChunkPhotons(G4int val) { fChunkPhotons = val; }
  G4int GetChunkPhotons() const { return fChunkPhotons; }

private:
  void DefineCommands();

private:

  long unsigned int fChunkPhotons;
  long unsigned int fChunkElectrons;

  TrackStack fPhotonStack;
  TrackStack fElectronStack;

  G4GenericMessenger* fMessenger;
};
