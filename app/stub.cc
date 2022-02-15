//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file stub.cc
//---------------------------------------------------------------------------//
#include "comm/Communicator.hh"
#include "comm/Logger.hh"
#include "comm/ScopedMpiInit.hh"

using namespace celeritas;

//---------------------------------------------------------------------------//
/*!
 * Execute and run.
 */
int main(int argc, char* argv[])
{
    ScopedMpiInit scoped_mpi(&argc, &argv);
    if (Communicator::comm_world().size() != 1)
    {
        CELER_LOG(critical) << "This app cannot run in parallel";
        return EXIT_FAILURE;
    }

    CELER_LOG(info) << "Not doing anything!";

    return EXIT_SUCCESS;
}

