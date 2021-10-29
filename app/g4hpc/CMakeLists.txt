#----------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#
#-----------------------------------------------------------------------------#
# g4hpc: a Geant4 tasking application for HEP (LHC/CMS)
#-----------------------------------------------------------------------------#
set(project g4hpc)

file(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/src/*.cc
                  ${CELERITAS_DIR}/app/demo-loop/LDemoParams.cc
                  ${CELERITAS_DIR}/app/demo-loop/LDemoRun.cc
                  ${CELERITAS_DIR}/app/demo-loop/LDemoKernel.cu
)

file(GLOB headers ${CMAKE_CURRENT_LIST_DIR}/include/*.hh
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include
                    ${CELERITAS_DIR}/app/demo-loop
                    ${CELERITAS_DIR}/external/nlohmann_json/include
)

add_executable(${project} ${project}.cc ${sources} ${headers})

celeritas_target_link_libraries(${project} celeritas
  Celeritas::Core
  Celeritas::ROOT
  VecGeom::vecgeom
  XercesC::XercesC
  ${Geant4_LIBRARIES})
