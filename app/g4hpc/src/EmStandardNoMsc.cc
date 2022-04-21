//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EmStandardNoMsc.cc
//---------------------------------------------------------------------------//
#include "EmStandardNoMsc.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmParameters.hh"
#include "G4EmBuilder.hh"
#include "G4LossTableManager.hh"

#include "G4ComptonScattering.hh"
#include "G4KleinNishinaModel.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermorePolarizedRayleighModel.hh"
#include "G4PhotoElectricAngularGeneratorPolarized.hh"

#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4NuclearStopping.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4GenericIon.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"
#include "G4GammaGeneralProcess.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(EmStandardNoMsc);

//---------------------------------------------------------------------------//
/*!
 * This class is a copy of G4EmStandardPhysics.cc of Geant4 11.0 and modified
 * for g4hpc.
 */
EmStandardNoMsc::EmStandardNoMsc(G4int ver, const G4String&)
    : G4VPhysicsConstructor("EmStandard")
{
    SetVerboseLevel(ver);
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetDefaults();
    param->SetVerbose(ver);
    SetPhysicsType(bElectromagnetic);
}

EmStandardNoMsc::~EmStandardNoMsc() {}

void EmStandardNoMsc::ConstructParticle()
{
    // minimal set of particles for EM physics
    G4EmBuilder::ConstructMinimalEmSet();
}

void EmStandardNoMsc::ConstructProcess()
{
    if (verboseLevel > 1)
    {
        G4cout << "### " << GetPhysicsName() << " Construct Processes "
               << G4endl;
    }
    G4EmBuilder::PrepareEMPhysics();

    G4PhysicsListHelper* ph    = G4PhysicsListHelper::GetPhysicsListHelper();
    G4EmParameters*      param = G4EmParameters::Instance();

    // processes used by several particles
    G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");

    // nuclear stopping is enabled if th eenergy limit above zero
    G4double           nielEnergyLimit = param->MaxNIELEnergy();
    G4NuclearStopping* pnuc            = nullptr;
    if (nielEnergyLimit > 0.0)
    {
        pnuc = new G4NuclearStopping();
        pnuc->SetMaxKinEnergy(nielEnergyLimit);
    }

    // Add gamma EM Processes
    G4ParticleDefinition* particle = G4Gamma::Gamma();
    G4bool                polar    = param->EnablePolarisation();

    // Photoelectric
    G4PhotoElectricEffect* pe      = new G4PhotoElectricEffect();
    G4VEmModel*            peModel = new G4LivermorePhotoElectricModel();
    pe->SetEmModel(peModel);
    if (polar)
    {
        peModel->SetAngularDistribution(
            new G4PhotoElectricAngularGeneratorPolarized());
    }

    // Compton scattering
    G4ComptonScattering* cs = new G4ComptonScattering;
    if (polar)
    {
        cs->SetEmModel(new G4KleinNishinaModel());
    }

    // default Rayleigh scattering is Livermore
    G4RayleighScattering* rl = new G4RayleighScattering();
    if (polar)
    {
        rl->SetEmModel(new G4LivermorePolarizedRayleighModel());
    }

    if (G4EmParameters::Instance()->GeneralProcessActive())
    {
        G4GammaGeneralProcess* sp = new G4GammaGeneralProcess();
        sp->AddEmProcess(pe);
        sp->AddEmProcess(cs);
        sp->AddEmProcess(new G4GammaConversion());
        sp->AddEmProcess(rl);
        G4LossTableManager::Instance()->SetGammaGeneralProcess(sp);
        ph->RegisterProcess(sp, particle);
    }
    else
    {
        ph->RegisterProcess(pe, particle);
        ph->RegisterProcess(cs, particle);
        ph->RegisterProcess(new G4GammaConversion(), particle);
        ph->RegisterProcess(rl, particle);
    }

    // e-
    particle = G4Electron::Electron();

    // No msc for e-

    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);

    // e+
    particle = G4Positron::Positron();

    // No msc for e+

    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);
    ph->RegisterProcess(new G4eplusAnnihilation(), particle);

    // generic ion
    particle                 = G4GenericIon::GenericIon();
    G4ionIonisation* ionIoni = new G4ionIonisation();
    ph->RegisterProcess(hmsc, particle);
    ph->RegisterProcess(ionIoni, particle);
    if (nullptr != pnuc)
    {
        ph->RegisterProcess(pnuc, particle);
    }

    // muons, hadrons ions
    G4EmBuilder::ConstructCharged(hmsc, pnuc);

    // extra configuration
    G4EmModelActivator mact(GetPhysicsName());
}
