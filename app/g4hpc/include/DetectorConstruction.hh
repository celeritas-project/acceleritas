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

class MagneticField;
class G4GenericMessenger;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  
  void SetMagField(G4double fieldValue);
  void SetGDMLFile();
  
private:
  
  static MagneticField* fMagField; 
  G4GenericMessenger* fMessenger = nullptr;
  
  G4String fFileName;
  std::unique_ptr<G4VPhysicalVolume> fWorldPhysVol;
};
