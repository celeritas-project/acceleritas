//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsList.hh
//---------------------------------------------------------------------------//
#pragma once

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

//---------------------------------------------------------------------------//
/*!
 * This class is a copy of FTFP_BERT.hh of Geant4 11.0 and modified for g4hpc.
 */
class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList(G4int ver = 1, G4int msc_option = 1);
    virtual ~PhysicsList() = default;

    PhysicsList(const PhysicsList&) = delete;
    PhysicsList& operator=(const PhysicsList&) = delete;
};
