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

if(NOT ACCELERITAS_BUILTIN_CELERITAS)
  find_package(Celeritas)
  if(NOT Celeritas_FOUND)
    message(STATUS "External celeritas not found: using built-in submodule")
    set(ACCELERITAS_BUILTIN_CELERITAS ON)
  endif()
endif()

option(ACCELERITAS_BUILTIN_CELERITAS "Use the built-in Celeritas submodule"
  "${ACCELERITAS_BUILTIN_CELERITAS}")

#---------------------------------------------------------------------------##
# DEPENDENCIES
#---------------------------------------------------------------------------##

find_package(Geant4 11 REQUIRED)
find_package(VecGeom 1.2 REQUIRED)

#---------------------------------------------------------------------------##
# EXTERNALS
#---------------------------------------------------------------------------##

if(ACCELERITAS_BUILTIN_CELERITAS)
  add_subdirectory(external)
endif()

#---------------------------------------------------------------------------##
# DEMO/HELPER APPS
#---------------------------------------------------------------------------##

add_subdirectory(app)

#---------------------------------------------------------------------------##
