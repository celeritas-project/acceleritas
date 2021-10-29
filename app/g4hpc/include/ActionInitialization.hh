//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    // Constructor and Destructors
    ActionInitialization()  = default;
    ~ActionInitialization() = default;

  public:
    void BuildForMaster() const final;
    void Build() const final;
};
