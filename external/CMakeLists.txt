#---------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other VecGeomTest Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

include(CeleritasLoadSubmodule)

#----------------------------------------------------------------------------#
# CELERITAS
#----------------------------------------------------------------------------#

if(ACCELERITAS_BUILTIN_CELERITAS)
  celeritas_load_submodule(celeritas)
  if(NOT APPLE)
    set(CELERITAS_USE_CUDA ON)
  endif()
  set(CELERITAS_USE_Geant4 ON)
  set(CELERITAS_USE_MPI OFF)
  set(CELERITAS_BUILD_DEMOS OFF)
  set(CELERITAS_BUILD_TESTS OFF)
  add_subdirectory(celeritas)
endif()

if(NOT TARGET Celeritas::Core)
  message(SEND_ERROR "Something is wrong: missing target Celeritas::Core "
    "(ACCELERITAS_BUILTIN_CELERITAS=${ACCELERITAS_BUILTIN_CELERITAS})")
endif()

#---------------------------------------------------------------------------##
