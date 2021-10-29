//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    G4VPhysicalVolume* Construct() final;
    void               ConstructSDandField() final;

  private:
    void SetGDMLFile();

  private:
    G4String                           fFileName;
    std::unique_ptr<G4VPhysicalVolume> fWorldPhysVol;
};
