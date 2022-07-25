//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackerHit.cc
//---------------------------------------------------------------------------//

#include "TrackerHit.hh"

G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator = nullptr;

TrackerHit::TrackerHit(id_type id, G4double time, G4ThreeVector pos)
    : G4VHit(), id_(id), time_(time), pos_(pos)
{
}

TrackerHit::~TrackerHit() {}

const TrackerHit& TrackerHit::operator=(const TrackerHit& rhs)
{
    id_   = rhs.id_;
    time_ = rhs.time_;
    pos_  = rhs.pos_;
    return *this;
}

G4bool TrackerHit::operator==(const TrackerHit& rhs) const
{
    return (this == &rhs) ? true : false;
}
