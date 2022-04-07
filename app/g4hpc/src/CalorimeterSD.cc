//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file CalorimeterSD.cc
//---------------------------------------------------------------------------//

#include "CalorimeterSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

CalorimeterSD::CalorimeterSD(G4String name)
    : G4VSensitiveDetector(name), hcid_(-1), collection_(nullptr)
{
    G4String HCname = name + "_HC";
    collectionName.insert(HCname);
}

void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
    collection_ = new CalorimeterHitsCollection(SensitiveDetectorName,
                                                collectionName[0]);
    if (hcid_ < 0)
    {
        hcid_ = G4SDManager::GetSDMpointer()->GetCollectionID(collection_);
    }
    hce->AddHitsCollection(hcid_, collection_);
}

G4bool CalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    auto edep = step->GetTotalEnergyDeposit();

    if (edep == 0.)
    {
        return false;
    }

    //  Get id, time, position for this sensitive detector
    auto         touchable = step->GetPreStepPoint()->GetTouchable();
    unsigned int id        = touchable->GetVolume()->GetCopyNo();
    auto         time      = step->GetPreStepPoint()->GetGlobalTime();
    auto         pos       = touchable->GetTranslation();

    //  Add energy deposition for this cell if it was hit before
    for (auto hit : *(collection_->GetVector()))
    {
        if (id == hit->id())
        {
            hit->add_edep(edep);
            return true;
        }
    }

    // Otherwise, create a new hit:
    collection_->insert(new CalorimeterHit(id, edep, time, pos));

    return true;
}
