#----------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
# g4hpc: a Geant4 tasking application for HEP (LHC/CMS)
#-----------------------------------------------------------------------------#
set(project g4hpc)

file(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/src/*.cc)
file(GLOB headers ${CMAKE_CURRENT_LIST_DIR}/include/*.hh)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(${project} ${project}.cc ${sources} ${headers})

celeritas_target_link_libraries(${project} celeritas
  Celeritas::Core
  XercesC::XercesC
  ${Geant4_LIBRARIES})
