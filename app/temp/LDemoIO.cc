//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file demo-loop/LDemoIO.cc
//---------------------------------------------------------------------------//
#include "LDemoIO.hh"

#include <algorithm>
#include <string>

#include "corecel/io/Logger.hh"
#include "corecel/io/StringUtils.hh"
#include "celeritas/em/process/BremsstrahlungProcess.hh"
#include "celeritas/em/process/ComptonProcess.hh"
#include "celeritas/em/process/EIonizationProcess.hh"
#include "celeritas/em/process/EPlusAnnihilationProcess.hh"
#include "celeritas/em/process/GammaConversionProcess.hh"
#include "celeritas/em/process/MultipleScatteringProcess.hh"
#include "celeritas/em/process/PhotoelectricProcess.hh"
#include "celeritas/em/process/RayleighProcess.hh"
#include "celeritas/ext/GeantImporter.hh"
#include "celeritas/ext/RootImporter.hh"
#include "celeritas/geo/GeoMaterialParams.hh"
#include "celeritas/geo/GeoParams.hh"
#include "celeritas/global/ActionManager.hh"
#include "celeritas/io/ImportData.hh"
#include "celeritas/mat/MaterialParams.hh"
#include "celeritas/phys/CutoffParams.hh"
#include "celeritas/phys/ImportedProcessAdapter.hh"
#include "celeritas/phys/ParticleParams.hh"
#include "celeritas/phys/PhysicsParams.hh"
#include "celeritas/random/RngParams.hh"

using namespace celeritas;

namespace demo_loop
{
namespace
{
//---------------------------------------------------------------------------//
// HELPER FUNCTIONS
//---------------------------------------------------------------------------//
//! Check that volume names are consistent between the ROOT file and geometry
bool volumes_are_consistent(const GeoParams&                 geo,
                            const std::vector<ImportVolume>& imported_data)
{
    return geo.num_volumes() == imported_data.size()
           && std::all_of(RangeIter<VolumeId>(VolumeId{0}),
                          RangeIter<VolumeId>(VolumeId{geo.num_volumes()}),
                          [&](VolumeId vol) {
                              return geo.id_to_label(vol)
                                     == imported_data[vol.unchecked_get()].name;
                          });
}
//---------------------------------------------------------------------------//
} // namespace

//---------------------------------------------------------------------------//
TransporterInput load_input(const LDemoArgs& args)
{
    CELER_LOG(status) << "Loading input and initializing problem data";
    TransporterInput  result;
    CoreParams::Input params;

    ImportData imported_data;
    if (ends_with(args.physics_filename, ".root"))
    {
        // Load imported_data from ROOT file
        imported_data = RootImporter(args.physics_filename.c_str())();
    }
    else if (ends_with(args.physics_filename, ".gdml"))
    {
        // Load imported_data directly from Geant4
        imported_data = GeantImporter(
            GeantSetup(args.physics_filename, args.geant_options))();
    }
    else
    {
        CELER_VALIDATE(false,
                       << "invalid physics filename '" << args.physics_filename
                       << "' (expected gdml or root)");
    }

    // Create action manager
    {
        ActionManager::Options opts;
        opts.sync         = args.sync;
        params.action_mgr = std::make_shared<ActionManager>(opts);
    }

    // Load geometry
    {
        params.geometry
            = std::make_shared<GeoParams>(args.geometry_filename.c_str());
        if (!params.geometry->supports_safety())
        {
            CELER_LOG(warning)
                << "Geometry contains surfaces that are "
                   "incompatible with the current ORANGE simple "
                   "safety algorithm: multiple scattering may "
                   "result in arbitrarily small steps";
        }
    }

    // Load materials
    {
        params.material = MaterialParams::from_import(imported_data);
    }

    // Create geometry/material coupling
    {
        GeoMaterialParams::Input input;
        input.geometry  = params.geometry;
        input.materials = params.material;

        input.volume_to_mat.resize(imported_data.volumes.size());
        for (auto volume_idx :
             range<VolumeId::size_type>(input.volume_to_mat.size()))
        {
            input.volume_to_mat[volume_idx]
                = MaterialId{imported_data.volumes[volume_idx].material_id};
        }
        if (!volumes_are_consistent(*input.geometry, imported_data.volumes))
        {
            // Volume names do not match exactly between exported ROOT file and
            // the geometry (possibly using a ROOT/GDML input with an ORANGE
            // geometry): try to let the GeoMaterialParams remap them
            CELER_LOG(warning) << "Volume/material mapping is inconsistent "
                                  "between Geant4 data and geometry file: "
                                  "attempting to remap";
            input.volume_names.resize(imported_data.volumes.size());
            for (auto volume_idx : range(imported_data.volumes.size()))
            {
                input.volume_names[volume_idx]
                    = std::move(imported_data.volumes[volume_idx].name);
            }
        }
        params.geomaterial
            = std::make_shared<GeoMaterialParams>(std::move(input));
    }

    // Construct particle params
    {
        params.particle = ParticleParams::from_import(imported_data);
    }

    // Construct cutoffs
    {
        params.cutoff = CutoffParams::from_import(
            imported_data, params.particle, params.material);
    }

    // Load physics: create individual processes with make_shared
    {
        PhysicsParams::Input input;
        input.particles                      = params.particle;
        input.materials                      = params.material;
        input.options.enable_fluctuation     = args.eloss_fluctuation;
        input.options.fixed_step_limiter     = args.step_limiter;
        input.options.secondary_stack_factor = args.secondary_stack_factor;
        input.action_manager                 = params.action_mgr.get();

        BremsstrahlungProcess::Options brem_options;
        brem_options.combined_model = args.brem_combined;
        brem_options.enable_lpm     = args.brem_lpm;

        GammaConversionProcess::Options conv_options;
        conv_options.enable_lpm = args.conv_lpm;

        auto process_data = std::make_shared<ImportedProcesses>(
            std::move(imported_data.processes));
        input.processes.push_back(
            std::make_shared<ComptonProcess>(params.particle, process_data));
        input.processes.push_back(std::make_shared<PhotoelectricProcess>(
            params.particle, params.material, process_data));
        if (args.rayleigh)
        {
            input.processes.push_back(std::make_shared<RayleighProcess>(
                params.particle, params.material, process_data));
        }
        input.processes.push_back(std::make_shared<GammaConversionProcess>(
            params.particle, process_data, conv_options));
        input.processes.push_back(
            std::make_shared<EPlusAnnihilationProcess>(params.particle));
        input.processes.push_back(std::make_shared<EIonizationProcess>(
            params.particle, process_data));
        input.processes.push_back(std::make_shared<BremsstrahlungProcess>(
            params.particle, params.material, process_data, brem_options));
        if (args.enable_msc)
        {
            input.processes.push_back(
                std::make_shared<MultipleScatteringProcess>(
                    params.particle, params.material, process_data));
        }
        params.physics = std::make_shared<PhysicsParams>(std::move(input));
    }

    // Construct RNG params
    {
        params.rng = std::make_shared<RngParams>(args.seed);
    }

    // Create params
    CELER_ASSERT(params);
    result.params = std::make_shared<CoreParams>(std::move(params));

    // Save constants
    CELER_VALIDATE(args.max_num_tracks > 0,
                   << "nonpositive max_num_tracks=" << args.max_num_tracks);
    CELER_VALIDATE(args.initializer_capacity > 0,
                   << "nonpositive initializer_capacity="
                   << args.initializer_capacity);
    CELER_VALIDATE(args.max_steps > 0,
                   << "nonpositive max_steps=" << args.max_steps);
    result.num_track_slots    = args.max_num_tracks;
    result.num_initializers   = args.initializer_capacity;
    result.max_steps          = args.max_steps;
    result.enable_diagnostics = args.enable_diagnostics;

    // Save diagnosics
    result.energy_diag = args.energy_diag;

    CELER_ENSURE(result);
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * Construct parameters, input, and transporter from the given run arguments.
 */
std::unique_ptr<TransporterBase> build_transporter(const LDemoArgs& run_args)
{
    using celeritas::MemSpace;

    TransporterInput                 input = load_input(run_args);
    std::unique_ptr<TransporterBase> result;

    if (run_args.use_device)
    {
        CELER_VALIDATE(celeritas::device(),
                       << "CUDA device is unavailable but GPU run was "
                          "requested");
        result = std::make_unique<Transporter<MemSpace::device>>(
            std::move(input));
    }
    else
    {
        result
            = std::make_unique<Transporter<MemSpace::host>>(std::move(input));
    }
    CELER_ENSURE(result);
    return result;
}

//---------------------------------------------------------------------------//
} // namespace demo_loop
