//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file CalorimeterSD.hh
//---------------------------------------------------------------------------//
#pragma once

#include "CalorimeterHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class CalorimeterSD : public G4VSensitiveDetector
{
  public:
    CalorimeterSD(G4String name);
    ~CalorimeterSD() = default;

    void   Initialize(G4HCofThisEvent*) final;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) final;

  private:
    G4int                      hcid_{-1};
    CalorimeterHitsCollection* collection_{nullptr};
};
