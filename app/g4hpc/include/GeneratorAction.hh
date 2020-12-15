//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeneratorAction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4ParticleGun;
class G4VPrimaryGenerator;

class GeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  GeneratorAction();    
  virtual ~GeneratorAction();
  
  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);
  
private:
  G4VPrimaryGenerator* HEPEvt;
  G4ParticleGun*  fParticleGun;

  G4bool   useHEPEvt;
  G4int    nParticle;
  G4double maxEta;
  G4double minEta;
  G4double maxPhi;
  G4double minPhi;
};
