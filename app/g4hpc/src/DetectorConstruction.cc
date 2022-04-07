//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//
#include "DetectorConstruction.hh"
#include "Configuration.hh"
#include "CalorimeterSD.hh"

#include "G4GDMLParser.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
    fParser = std::make_unique<G4GDMLParser>();
    SetGDMLFile();
    fParser.get()->Read(fFileName, false);
    fWorldPhysVol.reset(fParser.get()->GetWorldVolume());
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return fWorldPhysVol.release();
}

void DetectorConstruction::ConstructSDandField()
{
    // Add Sensitive detectors
    G4SDManager*            sd_manager = G4SDManager::GetSDMpointer();
    const G4GDMLAuxMapType* aux_map    = fParser.get()->GetAuxMap();
    for (auto& aux : *aux_map)
    {
        for (auto const& sd : aux.second)
        {
            if (sd.type == "SensDet" && sd.value == "em_calorimeter_sd")
            {
                G4String       name     = (aux.first)->GetName();
                CalorimeterSD* calor_sd = new CalorimeterSD(name);
                sd_manager->AddNewDetector(calor_sd);
                (aux.first)->SetSensitiveDetector(calor_sd);
            }
        }
    }

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
