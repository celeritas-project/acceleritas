//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeneratorMessenger.cc
//---------------------------------------------------------------------------//
#include "GeneratorMessenger.hh"

#include "GeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

GeneratorMessenger::GeneratorMessenger(GeneratorAction* generatorAction)
    : theGeneratorAction(generatorAction)
{
    theGeneratorDir = new G4UIdirectory("/mygen/");
    theGeneratorDir->SetGuidance("Generator control.");

    theGeneratorCmd = new G4UIcmdWithAString("/mygen/generator", this);
    theGeneratorCmd->SetGuidance("Select primary generator.");
    theGeneratorCmd->SetGuidance(
        "Available generators : hepEvent, particleGun");
    theGeneratorCmd->SetParameterName("generator", true);
    theGeneratorCmd->SetDefaultValue("hepEvent");
    theGeneratorCmd->SetCandidates("hepEvent particleGun");
}

GeneratorMessenger::~GeneratorMessenger()
{
    delete theGeneratorDir;
    delete theGeneratorCmd;
}

void GeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == theGeneratorCmd)
    {
        theGeneratorAction->SetUseHEPEvtGenerator(newValue == "hepEvent");
    }
}

G4String GeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
    G4String cv;

    if (command == theGeneratorCmd)
    {
        cv = (theGeneratorAction->GetUseHEPEvtGenerator()) ? "hepEvent"
                                                           : "particleGun";
    }

    return cv;
}
