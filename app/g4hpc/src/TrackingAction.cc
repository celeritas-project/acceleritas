//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "TrackingAction.hh"

#include "G4Track.hh"
#include "G4GenericMessenger.hh"

#include "physics/base/Primary.hh"

TrackingAction::TrackingAction()
  : G4UserTrackingAction(),
    fChunkPhotons(0),
    fChunkElectrons(0),
    fMessenger(nullptr)
{
  DefineCommands();
}

TrackingAction::~TrackingAction()
{ 
  delete fMessenger;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track) 
{
  unsigned int  pid = track->GetDefinition()->GetPDGEncoding();
  G4ThreeVector pos = track->GetPosition();
  G4ThreeVector dir = track->GetMomentumDirection();

  if(pid == 22) {
    fPhotonStack.push_back({ParticleDefId{pid},
	  units::MevEnergy{track->GetKineticEnergy()},
	    {pos.x(), pos.y(), pos.z()},
	    {dir.x(), dir.y(), dir.z()},
	    EventId{0}});

    (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);

  }
  if(pid == 11) {
    fElectronStack.push_back({ParticleDefId{pid},
	  units::MevEnergy{track->GetKineticEnergy()},
	    {pos.x(), pos.y(), pos.z()},
	    {dir.x(), dir.y(), dir.z()},
	    EventId{0}});

    (const_cast<G4Track*>(track))->SetTrackStatus(fStopAndKill);
  }

  //if size of the stack is fChunkElectrons
  if( fElectronStack.size() == fChunkElectrons ) {
    //TODO: lauch a kernel with this chuck of electrons 

    //reset 
    fElectronStack.clear();
  }

  //if size of the stack is fChunkPhotons
  if(fPhotonStack.size() == fChunkPhotons) {
    //TODO: lauch a kernel with this chuck of photons

    //reset 
    fPhotonStack.clear();
  }
}

void TrackingAction::DefineCommands()
{
  // Define the command directory
  fMessenger = new G4GenericMessenger(this, "/g4hpc/trackingAction/", 
                                      "User Tracking Action control");
            
  // commands for the size of paticle chunk for tasking
  auto& chunkElectronCmd
    = fMessenger->DeclareMethod("setChunkElectrons",
                                &TrackingAction::SetChunkElectrons, 
                                "Set the size of electron chunk for tasking");
  chunkElectronCmd.SetDefaultValue("10000");

  auto& chunkPhotonCmd
    = fMessenger->DeclareMethod("setChunkPhotons",
                                &TrackingAction::SetChunkPhotons, 
                                "Set the size of photon chunk for tasking");
  chunkPhotonCmd.SetDefaultValue("10000");
}
