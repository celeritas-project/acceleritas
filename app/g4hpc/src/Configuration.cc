//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Configuration.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

Configuration* Configuration::gInstance = nullptr;
MagneticField* Configuration::gMagField = nullptr;

Configuration* Configuration::Instance()
{
    if (gInstance == nullptr)
        gInstance = new Configuration();
    return gInstance;
}

Configuration::Configuration()
{
    DefineCommands();
}

Configuration::~Configuration()
{
    delete fMessenger;
    if (gMagField != nullptr)
        delete gMagField;
}

void Configuration::DefineCommands()
{
    // Define the command directory
    fMessenger = new G4GenericMessenger(this, "/g4hpc/", "g4hpc control");

    // DetectorConstruction - magnetic field
    auto& setMagFieldCmd = fMessenger->DeclareMethodWithUnit(
        "setMagField",
        "tesla",
        &Configuration::SetMagField,
        "Set the magnetic field value (along the x-axis");
    setMagFieldCmd.SetWorkerThreadOnly(false);

    // GeneratorAction
    auto& generatorCmd
        = fMessenger->DeclareProperty("useHEPEvtGenerator", fUseHEPEvt);
    generatorCmd.SetGuidance("Use HEPEvt Generator");
    generatorCmd.SetDefaultValue("true");

    // IO
    auto& writeHitCmd = fMessenger->DeclareProperty("setWriteHits", fWriteHits);
    writeHitCmd.SetGuidance("Set the WriteHits flag");
    writeHitCmd.SetDefaultValue("false");

    // Tracking Action
    auto& numUserThreadsCmd
        = fMessenger->DeclareProperty("setNumUserThreads", fNumUserThreads);
    numUserThreadsCmd.SetGuidance("Set the size of user task threads");
    numUserThreadsCmd.SetDefaultValue("1");

    auto& chunkTracksCmd
        = fMessenger->DeclareProperty("setChunkTracks", fChunkTracks);
    chunkTracksCmd.SetGuidance("Set the size of track chunk for tasking");
    chunkTracksCmd.SetDefaultValue("10000");

    // Parameters for celeritas
    auto& geometryFileCmd
        = fMessenger->DeclareProperty("setGeometryFile", fGeometryFile);
    geometryFileCmd.SetGuidance("Set the geometry file name");
    geometryFileCmd.SetDefaultValue("simple-cms.gdml");

    auto& physicsFileCmd
        = fMessenger->DeclareProperty("setPhysicsFile", fPhysicsFile);
    physicsFileCmd.SetGuidance("Set the physics file name");
    physicsFileCmd.SetDefaultValue("simple-cms.gdml.root");

    auto& hepmc3FileCmd
        = fMessenger->DeclareProperty("setHepMC3File", fHepMC3File);
    hepmc3FileCmd.SetGuidance("Set the HepMC3 file name");
    hepmc3FileCmd.SetDefaultValue("input.hepmc3");

    auto& seedCmd = fMessenger->DeclareProperty("setSeed", fSeed);
    seedCmd.SetGuidance("Set the seed for celeritas");
    seedCmd.SetDefaultValue("123456");

    auto& maxNumTracksCmd
        = fMessenger->DeclareProperty("setMaxNumTracks", fMaxNumTracks);
    maxNumTracksCmd.SetGuidance("Set the maximum number of tracks");
    maxNumTracksCmd.SetDefaultValue("4096");

    auto& maxStepsCmd = fMessenger->DeclareProperty("setMaxSteps", fMaxSteps);
    maxStepsCmd.SetGuidance("Set the maximum number of steps");
    maxStepsCmd.SetDefaultValue("128");

    auto& capacityCmd = fMessenger->DeclareProperty("setCapacity", fCapacity);
    capacityCmd.SetGuidance("Set the capacity factor");
    capacityCmd.SetDefaultValue("1000000");

    auto& secondaryStackFactorCmd = fMessenger->DeclareProperty(
        "setSecondaryStackFactor", fSecondaryStackFactor);
    secondaryStackFactorCmd.SetGuidance("Set the secondary stack factor");
    secondaryStackFactorCmd.SetDefaultValue("3");

    auto& useDeviceCmd
        = fMessenger->DeclareProperty("setUseDevice", fUseDevice);
    useDeviceCmd.SetGuidance("Set the use device flag");
    useDeviceCmd.SetDefaultValue("true");

    auto& enableDiagnosticsCmd = fMessenger->DeclareProperty(
        "setEnableDiagnostics", fEnableDiagnostics);
    enableDiagnosticsCmd.SetGuidance("Set the enable diagnostics  flag");
    enableDiagnosticsCmd.SetDefaultValue("false");

    auto& syncCmd = fMessenger->DeclareProperty("setSync", fSync);
    syncCmd.SetGuidance("Set the sync flag");
    syncCmd.SetDefaultValue("true");

    auto& offLoadCmd = fMessenger->DeclareProperty("setOffLoad", fOffLoad);
    offLoadCmd.SetGuidance("Set the offload flag");
    offLoadCmd.SetDefaultValue("true");

    auto& enableAnalysisCmd
        = fMessenger->DeclareProperty("setEnableAnalysis", fEnableAnalysis);
    enableAnalysisCmd.SetGuidance("Set the analysis flag");
    enableAnalysisCmd.SetDefaultValue("false");
}

void Configuration::SetMagField(G4double fieldValue)
{
    // Apply a global uniform magnetic field along the x-axis
    G4FieldManager* fieldManager
        = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    if (fieldValue != 0.)
    {
        gMagField = new MagneticField();
        gMagField->SetFlux(fieldValue);

        fieldManager->SetDetectorField(gMagField);
        fieldManager->CreateChordFinder(gMagField);
    }
    else
    {
        fieldManager->SetDetectorField(gMagField);
    }
}
