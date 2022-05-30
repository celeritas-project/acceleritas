//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file demo-loop/Transporter.cc
//---------------------------------------------------------------------------//
#include "Transporter.hh"

#include <csignal>
#include <memory>
#include <type_traits>

#include "corecel/Assert.hh"
#include "corecel/data/Ref.hh"
#include "corecel/io/Logger.hh"
#include "corecel/math/VectorUtils.hh"
#include "corecel/sys/ScopedSignalHandler.hh"
#include "corecel/sys/Stopwatch.hh"
#include "celeritas/global/ActionManager.hh"
#include "celeritas/global/Stepper.hh"

using namespace celeritas;

namespace demo_loop
{
namespace
{
//---------------------------------------------------------------------------//
// HELPER CLASSES AND FUNCTIONS
//---------------------------------------------------------------------------//
template<MemSpace M>
using MemTag = std::integral_constant<MemSpace, M>;


//---------------------------------------------------------------------------//
} // namespace

//---------------------------------------------------------------------------//
//! Default virtual destructor
TransporterBase::~TransporterBase() = default;

//---------------------------------------------------------------------------//
/*!
 * Construct from persistent problem data.
 */
template<MemSpace M>
Transporter<M>::Transporter(TransporterInput inp)
{
    TransporterBase::input_ = std::move(inp);
    CELER_EXPECT(input_);

    // Create diagnostics
    CELER_VALIDATE(!input_.enable_diagnostics,
                   << "diagnostics cannot be enabled for acceleritas");
}

//---------------------------------------------------------------------------//
/*!
 * Transport the input primaries and all secondaries produced.
 */
template<MemSpace M>
TransporterResult Transporter<M>::operator()(VecPrimary primaries)
{
    Stopwatch get_transport_time;

    // Initialize results
    TransporterResult result;
    if (input_.max_steps != input_.no_max_steps())
    {
        result.time.steps.reserve(input_.max_steps);
        result.initializers.reserve(input_.max_steps);
        result.active.reserve(input_.max_steps);
        result.alive.reserve(input_.max_steps);
    }
    auto append_track_counts = [&result](const StepperResult& track_counts) {
        result.initializers.push_back(track_counts.queued);
        result.active.push_back(track_counts.active);
        result.alive.push_back(track_counts.alive);
    };

    // Abort cleanly for interrupt and user-defined signals
    ScopedSignalHandler interrupted{SIGINT, SIGUSR2};
    CELER_LOG(status) << "Transporting";

    StepperInput input;
    input.params             = input_.params;
    input.num_track_slots    = input_.num_track_slots;
    input.num_initializers   = input_.num_initializers;
    Stepper<M> step(std::move(input));

    Stopwatch get_step_time;
    size_type remaining_steps = input_.max_steps;

    // Copy primaries to device and transport the first step
    auto      track_counts    = step(std::move(primaries));
    append_track_counts(track_counts);
    result.time.steps.push_back(get_step_time());

    while (track_counts)
    {
        if (CELER_UNLIKELY(--remaining_steps == 0))
        {
            CELER_LOG(error) << "Exceeded step count of " << input_.max_steps
                             << ": aborting transport loop";
            break;
        }
        if (CELER_UNLIKELY(interrupted()))
        {
            CELER_LOG(error) << "Caught interrupt signal: aborting transport "
                                "loop";
            interrupted = {};
            break;
        }

        get_step_time = {};
        track_counts  = step();
        append_track_counts(track_counts);
        result.time.steps.push_back(get_step_time());
    }

    result.time.total = get_transport_time();
    return result;
}

//---------------------------------------------------------------------------//
// EXPLICIT INSTANTIATION
//---------------------------------------------------------------------------//

template class Transporter<MemSpace::host>;
template class Transporter<MemSpace::device>;

//---------------------------------------------------------------------------//
} // namespace demo_loop
