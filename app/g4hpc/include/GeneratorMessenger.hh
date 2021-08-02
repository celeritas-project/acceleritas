//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeneratorMessenger.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class GeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class GeneratorMessenger : public G4UImessenger
{
  public:
    GeneratorMessenger(GeneratorAction* generatorAction);
    ~GeneratorMessenger();

    void     SetNewValue(G4UIcommand* command, G4String newValue);
    G4String GetCurrentValue(G4UIcommand* command);

  private:
    GeneratorAction*    theGeneratorAction;
    G4UIdirectory*      theGeneratorDir;
    G4UIcmdWithAString* theGeneratorCmd;
};
