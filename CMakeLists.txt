#---------------------------------*-CMake-*----------------------------------#
# Copyright 2021-2022 UT-Battelle, LLC and other Celeritas Developers.
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
include(CMakeDependentOption)

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
  find_package(VecGeom 1.1.17 REQUIRED)

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
  find_package(HepMC3 REQUIRED)
endif()

if(CELERITAS_USE_JSON)
  find_package(nlohmann_json 3.7.0 REQUIRED)
endif()

#---------------------------------------------------------------------------##
# DEMO/HELPER APPS
#---------------------------------------------------------------------------##

add_subdirectory(app)

#---------------------------------------------------------------------------##
