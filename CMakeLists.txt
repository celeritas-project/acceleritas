#---------------------------------*-CMake-*----------------------------------#
# Copyright 2021-2022 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.12)
project(Acceleritas VERSION 0.0.0 LANGUAGES CXX)
cmake_policy(VERSION 3.12...3.18)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)

#---------------------------------------------------------------------------##
# EXTERNAL CELERITAS
#---------------------------------------------------------------------------##

set(_default_builtin OFF)
if(NOT ACCELERITAS_BUILTIN_CELERITAS)
  find_package(Celeritas 0.2 REQUIRED)
  if(NOT Celeritas_FOUND)
    message(STATUS "External celeritas not found: using built-in submodule")
    set(_default_builtin ON)
  endif()
endif()

option(ACCELERITAS_BUILTIN_CELERITAS "Use the built-in Celeritas submodule"
  "${_default_builtin}")

if(ACCELERITAS_BUILTIN_CELERITAS)
  add_subdirectory(external)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/celeritas/cmake")
  # CUDA toolkit doesn't
endif()

#---------------------------------------------------------------------------##
# DEPENDENCIES
#---------------------------------------------------------------------------##

if(NOT Geant4_FOUND)
  find_package(Geant4 11 REQUIRED)
elseif(NOT Geant4_VERSION VERSION_GREATER_EQUAL 11)
  message(SEND_ERROR "Geant4 version is too old")
endif()

if(CELERITAS_USE_CUDA AND ACCELERITAS_BUILTIN_CELERITAS)
  # For some reason the CUDA toolkit target is not global?
  find_package(CUDAToolkit QUIET REQUIRED)
endif()

#---------------------------------------------------------------------------##
# DEMO/HELPER APPS
#---------------------------------------------------------------------------##

add_subdirectory(app)

#---------------------------------------------------------------------------##
