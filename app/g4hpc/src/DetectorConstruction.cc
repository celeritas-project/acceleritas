//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//
#include "DetectorConstruction.hh"
#include "Configuration.hh"

#include "G4GDMLParser.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
    G4GDMLParser parser;
    SetGDMLFile();
    parser.Read(fFileName, false);
    fWorldPhysVol.reset(parser.GetWorldVolume());
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return fWorldPhysVol.release();
}

void DetectorConstruction::ConstructSDandField()
{
    // The magnetic field is defined through the UI messenger
    Configuration::Instance()->SetMagField(0.);
}

void DetectorConstruction::SetGDMLFile()
{
    char* gdmlFile = getenv("G4HPC_GDML");
    if (gdmlFile)
    {
        fFileName = gdmlFile;
    }
    else
    {
        // The default geometry
        fFileName = "input/simple-cms.gdml";
    }
}
