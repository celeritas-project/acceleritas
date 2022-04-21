//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EmStandardNoMsc.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//---------------------------------------------------------------------------//
/*!
 * This class is a copy of G4EmStandardPhysics.hh of Geant4 11.0 and modified
 * for g4hpc.
 */
class EmStandardNoMsc : public G4VPhysicsConstructor
{
  public:
    explicit EmStandardNoMsc(G4int ver = 1, const G4String& name = "");

    ~EmStandardNoMsc() override;

    void ConstructParticle() override;
    void ConstructProcess() override;
};
