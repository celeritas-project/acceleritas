#---------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other VecGeomTest Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

#----------------------------------------------------------------------------#
# CELERITAS
#----------------------------------------------------------------------------#

if(NOT TARGET Celeritas::Core)
  celeritas_load_submodule(celeritas)
  set(CELERITAS_USE_CUDA ON)
  set(CELERITAS_USE_Geant4 ON)
  set(CELERITAS_USE_MPI OFF)
  set(CELERITAS_BUILD_DEMOS OFF)
  set(CELERITAS_BUILD_TESTS OFF)
  add_subdirectory(celeritas)
endif()

#---------------------------------------------------------------------------##
