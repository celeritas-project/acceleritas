//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file CalorimeterHit.hh
//---------------------------------------------------------------------------//
#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class CalorimeterHit : public G4VHit
{
    using id_type = unsigned int;

  public:
    CalorimeterHit() : G4VHit() {}
    CalorimeterHit(id_type id, G4double edep, G4double time, G4ThreeVector pos);
    ~CalorimeterHit() override;

    CalorimeterHit(const CalorimeterHit&) = default;
    const CalorimeterHit& operator        =(const CalorimeterHit& rhs);
    G4bool                operator==(const CalorimeterHit& rhs) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Accessors
    inline id_type       id() const { return id_; };
    inline G4double      edep() const { return edep_; };
    inline G4double      time() const { return time_; };
    inline G4ThreeVector pos() const { return pos_; };

    inline void add_edep(G4double edep) { edep_ += edep; };

  private:
    id_type       id_{0};
    G4double      edep_{0};
    G4double      time_{0};
    G4ThreeVector pos_{0, 0, 0};
};

using CalorimeterHitsCollection = G4THitsCollection<CalorimeterHit>;
extern G4ThreadLocal G4Allocator<CalorimeterHit>* CalorimeterHitAllocator;

inline void* CalorimeterHit::operator new(size_t)
{
    if (!CalorimeterHitAllocator)
    {
        CalorimeterHitAllocator = new G4Allocator<CalorimeterHit>;
    }
    return (void*)CalorimeterHitAllocator->MallocSingle();
}

inline void CalorimeterHit::operator delete(void* hit)
{
    CalorimeterHitAllocator->FreeSingle((CalorimeterHit*)hit);
}
