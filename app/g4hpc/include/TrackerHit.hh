//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackerHit.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class TrackerHit : public G4VHit
{
    using id_type = unsigned int;

  public:
    TrackerHit() : G4VHit() {}
    TrackerHit(id_type id, G4double time, G4ThreeVector pos);
    ~TrackerHit() override;

    TrackerHit(const TrackerHit&) = default;
    const TrackerHit& operator    =(const TrackerHit& rhs);
    G4bool            operator==(const TrackerHit& rhs) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Accessors
    inline id_type       id() const { return id_; };
    inline G4double      time() const { return time_; };
    inline G4ThreeVector pos() const { return pos_; };

  private:
    id_type       id_{0};
    G4double      time_{0};
    G4ThreeVector pos_{0, 0, 0};
};

using TrackerHitsCollection = G4THitsCollection<TrackerHit>;
extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
    if (!TrackerHitAllocator)
    {
        TrackerHitAllocator = new G4Allocator<TrackerHit>;
    }
    return (void*)TrackerHitAllocator->MallocSingle();
}

inline void TrackerHit::operator delete(void* hit)
{
    TrackerHitAllocator->FreeSingle((TrackerHit*)hit);
}
