#----------------------------------*-CMake-*----------------------------------#
# Copyright 2022 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

if(NOT ACCELERITAS_BUILTIN_CELERITAS)
  include(CeleritasLibrary)
endif()

#-----------------------------------------------------------------------------#
# acc02: a Geant4 event-level multithreaded application for HEP (LHC/CMS)
#-----------------------------------------------------------------------------#
add_executable(acc02
  acc02.cc
  src/EventAction.cc
  src/RunAction.cc
  src/TrackingAction.cc
  src/DetectorConstruction.cc
  ../acc01/src/ActionInitialization.cc
  ../acc01/src/GeneratorAction.cc
  ../acc01/src/Configuration.cc
  ../acc01/src/MagneticField.cc
  ../temp/LDemoIO.cc
  ../temp/Transporter.cc
)

target_include_directories(acc02 PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/../temp
  ${CMAKE_CURRENT_LIST_DIR}/../acc01/include
)

celeritas_target_link_libraries(acc02
  Celeritas::Core
  Celeritas::IO
  ${Geant4_LIBRARIES}
)

if(CELERITAS_USE_CUDA)
  # Since we call cudaDeviceSync
  celeritas_target_link_libraries(acc02
    CUDA::cudart
  )
endif()
