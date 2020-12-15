//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file g4hpc.cc
//---------------------------------------------------------------------------//
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "G4RunManagerFactory.hh"
#include "G4Threading.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"


// for std::system(const char*)
//#include <cstdlib>

int main(int argc, char** argv)
{
  if(argc != 2) {
    G4cout << "Usage: g4hpc run.mac" << G4endl;
    return -1;
  }

  // Set the random seed
  CLHEP::HepRandom::setTheSeed(1245214UL);

  auto* runmanager =
  G4RunManagerFactory::CreateRunManager(G4RunManagerType::Tasking);

  // Set user actions and initialize run

  auto detector = std::make_unique<DetectorConstruction>();
  runmanager->SetUserInitialization(detector.release());

  auto physics_list = std::make_unique<FTFP_BERT>();
  runmanager->SetUserInitialization(physics_list.release());

  auto action_init = std::make_unique<ActionInitialization>();
  runmanager->SetUserInitialization(action_init.release());

  runmanager->Initialize();

  // Set the user interface
  G4UImanager* UI = G4UImanager::GetUIpointer();

  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command+fileName);

  //@termination
  delete runmanager;

  return 0;
}
