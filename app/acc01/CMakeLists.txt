#----------------------------------*-CMake-*----------------------------------#
# Copyright 2021-2022 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

if(NOT ACCELERITAS_BUILTIN_CELERITAS)
  include(CeleritasLibrary)
endif()

#-----------------------------------------------------------------------------#
# acc01: a Geant4 tasking application for HEP (LHC/CMS)
#-----------------------------------------------------------------------------#
add_executable(acc01
  acc01.cc
  src/ActionInitialization.cc
  src/CalorimeterHit.cc
  src/CalorimeterSD.cc
  src/Configuration.cc
  src/DetectorConstruction.cc
  src/DeviceAction.cc
  src/DeviceManager.cc
  src/EmStandardNoMsc.cc
  src/EventAction.cc
  src/GeneratorAction.cc
  src/MagneticField.cc
  src/PhysicsList.cc
  src/RunAction.cc
  src/TrackerHit.cc
  src/TrackerSD.cc
  src/TrackingAction.cc
  ../temp/LDemoIO.cc
  ../temp/Transporter.cc
)

target_include_directories(acc01 PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/../temp
)

celeritas_target_link_libraries(acc01
  Celeritas::celeritas
  nlohmann_json::nlohmann_json
  ${Geant4_LIBRARIES}
)

if(CELERITAS_USE_CUDA)
  # Since we call cudaDeviceSync
  celeritas_target_link_libraries(acc01
    CUDA::cudart
  )
endif()
