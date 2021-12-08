#---------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.12)
project(AcceleritasGeant4 VERSION 0.0.1 LANGUAGES CXX CUDA)
cmake_policy(VERSION 3.12...3.18)

set(CELERITAS_DIR ${PROJECT_SOURCE_DIR}/external/celeritas)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake"
                              "${CELERITAS_DIR}/cmake")

include(GNUInstallDirs)
include(CeleritasUtils)

#---------------------------------------------------------------------------##
# OPTIONS
#---------------------------------------------------------------------------##
option(CELERITAS_USE_VecGeom "Enable VecGeom geometry" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_EXTENSIONS OFF)

#---------------------------------------------------------------------------##
# DEPENDENCIES
#---------------------------------------------------------------------------##

find_package(Geant4 REQUIRED)

if(CELERITAS_USE_VecGeom)
  celeritas_find_package_config(VecGeom 1.1.13 REQUIRED)

  if((CELERITAS_USE_CUDA AND NOT VecGeom_CUDA_FOUND)
      OR (NOT CELERITAS_USE_CUDA AND VecGeom_CUDA_FOUND))
    message(SEND_ERROR "CUDA mismatch between the VecGeom installation "
      "at ${VECGEOM_INSTALL_DIR} (VecGeom_CUDA_FOUND=${VecGeom_CUDA_FOUND}) "
      "and Celeritas (CELERITAS_USE_CUDA=${CELERITAS_USE_CUDA})"
    )
  endif()
  if(NOT VecGeom_GDML_FOUND)
    message(SEND_ERROR "VecGeom GDML capability is required for Celeritas")
  endif()

  set_target_properties(VecGeom::vecgeom PROPERTIES
    CELERITAS_CUDA_STATIC_LIBRARY VecGeom::vecgeomcuda_static
    CELERITAS_CUDA_MIDDLE_LIBRARY VecGeom::vecgeomcuda
    CELERITAS_CUDA_FINAL_LIBRARY VecGeom::vecgeomcuda
  )
  set_target_properties(VecGeom::vecgeomcuda PROPERTIES
    CELERITAS_CUDA_LIBRARY_TYPE Shared
    CELERITAS_CUDA_STATIC_LIBRARY VecGeom::vecgeomcuda_static
    CELERITAS_CUDA_MIDDLE_LIBRARY VecGeom::vecgeomcuda
    CELERITAS_CUDA_FINAL_LIBRARY VecGeom::vecgeomcuda
  )
  set_target_properties(VecGeom::vecgeomcuda_static PROPERTIES
    CELERITAS_CUDA_LIBRARY_TYPE Static
    CELERITAS_CUDA_STATIC_LIBRARY VecGeom::vecgeomcuda_static
    CELERITAS_CUDA_MIDDLE_LIBRARY VecGeom::vecgeomcuda
    CELERITAS_CUDA_FINAL_LIBRARY VecGeom::vecgeomcuda
  )
endif()

find_package(OpenMP)

#---------------------------------------------------------------------------##
# EXTERNALS
#---------------------------------------------------------------------------##
include(CeleritasLoadSubmodule)

if(NOT CELERITAS_GIT_SUBMODULE)
  set(_required_when_no_git REQUIRED)
else()
  set(_required_when_no_git QUIET)
endif()

find_package(Celeritas ${_required_when_no_git})

add_subdirectory(external)

if(CELERITAS_USE_HepMC3)
  celeritas_find_package_config(HepMC3 REQUIRED)
  # HepMC3's cmake config file does not provide an imported target yet
  # Anticipate that it will, and create if not present. HepMC3 is
  # a private dep of Celeritas, so no need to re-export this.
  if(NOT TARGET HepMC3::HepMC3)
    add_library(HepMC3::HepMC3 UNKNOWN IMPORTED)
    set_target_properties(HepMC3::HepMC3 PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${HEPMC3_INCLUDE_DIR}"
      IMPORTED_LOCATION "${HEPMC3_LIB}"
    )
  endif()
endif()

#---------------------------------------------------------------------------##
# DEMO/HELPER APPS
#---------------------------------------------------------------------------##

add_subdirectory(app)

#---------------------------------------------------------------------------##
