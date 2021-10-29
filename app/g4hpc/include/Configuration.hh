//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Configuration.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"

class MagneticField;
class G4GenericMessenger;

class Configuration
{
    using size_type = long unsigned int;

  public:
    static Configuration* Instance();
    ~Configuration();

    void   SetMagField(G4double fieldValue);
    G4bool GetUseHEPEvtGenerator() const { return fUseHEPEvt; }

    size_type GetNumUserThreads() const { return fNumUserThreads; }
    size_type GetChunkTracks() const { return fChunkTracks; }

    G4String GetGeometryFile() const { return fGeometryFile; }
    G4String GetPhysicsFile() const { return fPhysicsFile; }
    G4String GetHepMC3File() const { return fHepMC3File; }

    size_type GetSeed() const { return fSeed; }
    size_type GetMaxNumTracks() const { return fMaxNumTracks; }
    size_type GetMaxSteps() const { return fMaxSteps; }
    size_type GetStorageFactor() const { return fStorageFactor; }

  private:
    Configuration();
    void DefineCommands();

  private:
    static Configuration* gInstance;
    static MagneticField* gMagField;

    G4bool    fUseHEPEvt{false};
    size_type fNumUserThreads{1};
    size_type fChunkTracks{0};

    G4String  fGeometryFile = "simple_cms.gdml";
    G4String  fPhysicsFile  = "simple_cms.gdml.root";
    G4String  fHepMC3File   = "input.hepmc3";
    size_type fSeed{};
    size_type fMaxNumTracks{};
    size_type fMaxSteps{};
    size_type fStorageFactor{};

    G4GenericMessenger* fMessenger = nullptr;
};
