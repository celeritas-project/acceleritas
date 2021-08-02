#---------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.12)
project(AcceleritasGeant4 VERSION 0.0.1 LANGUAGES CXX CUDA)
cmake_policy(VERSION 3.12...3.18)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)

#---------------------------------------------------------------------------##
# OPTIONS
#---------------------------------------------------------------------------##

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_EXTENSIONS OFF)

#---------------------------------------------------------------------------##
# DEPENDENCIES
#---------------------------------------------------------------------------##

find_package(Geant4 REQUIRED)

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

#---------------------------------------------------------------------------##
# DEMO/HELPER APPS
#---------------------------------------------------------------------------##

add_subdirectory(app)

#---------------------------------------------------------------------------##
