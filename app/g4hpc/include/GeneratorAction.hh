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
    explicit GeneratorAction();
    ~GeneratorAction() = default;

    void GeneratePrimaries(G4Event* event) final;

  private:
    G4ParticleGun*       fParticleGun = nullptr;
    G4VPrimaryGenerator* fHEPEvt      = nullptr;
};
