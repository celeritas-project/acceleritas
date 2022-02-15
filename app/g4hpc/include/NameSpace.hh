//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file NameSpace.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"

namespace g4hpc
{
// tracker volume for magnetic field: unit in [mm]
const G4double trackerRmax = 3000.0;
const G4double trackerZmax = 3000.0;

// detector volume for magnetic field: unit in [mm]
const G4double minR = 0.0;
const G4double maxR = 9000.0;
const G4double minZ = -16000.0;
const G4double maxZ = 16000.0;

// field map array
const G4int nbinR = 900 + 1;
const G4int nbinZ = 2 * 1600 + 1;
const G4int noffZ = 1600;

// Magnetic field types

enum BFieldType
{
    kNull = -1,  // Not defined
    kVolumebase, // Volumebase grid (default for all regions)
    kUniform,    // Uniform mangnetic field along +z
    kNumberBFieldType
};

struct cmsFieldMapData
{
    G4int iz;
    G4int ir;
    float Bz;
    float Br;
};

} // namespace g4hpc
