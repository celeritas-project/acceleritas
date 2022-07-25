//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "Configuration.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"

void EventAction::BeginOfEventAction(const G4Event* event)
{
    fTrackingAction->SetEventID(event->GetEventID());
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Get hits collections IDs (only once)
    if (fcalorHCID == -1)
    {
        fcalorHCID = G4SDManager::GetSDMpointer()->GetCollectionID(
            "em_calorimeter_HC");
    }

    // Get hits collections
    auto calor_hc = GetHitsCollection(fcalorHCID, event);

    if (Configuration::Instance()->GetEnableAnalysis()
        && calor_hc->entries() > 0)
    {
        // Get hit with total values
        auto calor_hit = (*calor_hc)[calor_hc->entries() - 1];

        // Do analysis
        auto analysis_manager = G4AnalysisManager::Instance();
        analysis_manager->FillH1(0, calor_hit->edep());
    }
}

CalorimeterHitsCollection*
EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection = static_cast<CalorimeterHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

    if (!hitsCollection)
    {
        G4ExceptionDescription msg;
        msg << "Cannot access hits Collection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()",
                    "MyCode0003",
                    FatalException,
                    msg);
    }

    return hitsCollection;
}
