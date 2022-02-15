#----------------------------------*-CMake-*----------------------------------#
# Copyright 2021-2022 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

add_executable(stub stub.cc)
celeritas_target_link_libraries(stub
  Celeritas::Core
  ${Geant4_LIBRARIES})

add_subdirectory(g4hpc)
#-----------------------------------------------------------------------------#
