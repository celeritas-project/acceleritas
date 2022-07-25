//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file CalorimeterHit.cc
//---------------------------------------------------------------------------//

#include "CalorimeterHit.hh"

G4ThreadLocal G4Allocator<CalorimeterHit>* CalorimeterHitAllocator = nullptr;

CalorimeterHit::CalorimeterHit(id_type       id,
                               G4double      edep,
                               G4double      time,
                               G4ThreeVector pos)
    : G4VHit(), id_(id), edep_(edep), time_(time), pos_(pos)
{
}

CalorimeterHit::~CalorimeterHit() {}

const CalorimeterHit& CalorimeterHit::operator=(const CalorimeterHit& rhs)
{
    id_   = rhs.id_;
    edep_ = rhs.edep_;
    time_ = rhs.time_;
    pos_  = rhs.pos_;
    return *this;
}

G4bool CalorimeterHit::operator==(const CalorimeterHit& rhs) const
{
    return (this == &rhs) ? true : false;
}
