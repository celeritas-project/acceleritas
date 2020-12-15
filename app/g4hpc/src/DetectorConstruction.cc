//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//
#include "DetectorConstruction.hh"
#include "MagneticField.hh"

#include "G4GDMLParser.hh"
#include "G4FieldManager.hh"
#include "G4GenericMessenger.hh"
#include "G4TransportationManager.hh"

MagneticField* DetectorConstruction::fMagField = nullptr;

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  G4GDMLParser parser;
  SetGDMLFile();
  parser.Read(fFileName,false);
  fWorldPhysVol.reset(parser.GetWorldVolume());
}

DetectorConstruction::~DetectorConstruction()
{ 
  delete fMagField;
  delete fMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return fWorldPhysVol.release();
}

void DetectorConstruction::ConstructSDandField()
{ 
  // No field is defined by default
  SetMagField(0.); 

  // Define /g4task/det/setMagField command
  fMessenger = new G4GenericMessenger(this, "/g4hpc/det/", 
				      "Detector construction control");
  auto& setMagFieldCmd
    = fMessenger->DeclareMethodWithUnit("setMagField","tesla", 
					&DetectorConstruction::SetMagField, 
					"Magnetic field (along the x-axis");
 
  setMagFieldCmd.SetWorkerThreadOnly(false); 
}

void DetectorConstruction::SetMagField(G4double fieldValue)
{
  // Apply a global uniform magnetic field along the x-axis
  G4FieldManager* fieldManager
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  
  if ( fieldValue != 0. ) {
    fMagField = new MagneticField();
    fMagField->SetFlux(fieldValue);
    
    fieldManager->SetDetectorField(fMagField);
    fieldManager->CreateChordFinder(fMagField);
  } 
  else {
    fMagField = 0;
    fieldManager->SetDetectorField(fMagField);
  }
}

void DetectorConstruction::SetGDMLFile() 
{
  char* gdmlFile = getenv("G4HPC_GDML");
  if (gdmlFile) {
    fFileName = gdmlFile;
  }
  else {
    // the default geometry
    fFileName ="input/cms.gdml";
  }
}
