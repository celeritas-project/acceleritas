#----------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

add_executable(stub stub.cc)
target_link_libraries(stub
  Celeritas::Core
  ${Geant4_LIBRARIES})

#-----------------------------------------------------------------------------#
