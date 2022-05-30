#----------------------------------*-CMake-*----------------------------------#
# Copyright 2021-2022 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

include(CeleritasLibrary)

#-----------------------------------------------------------------------------#
# g4hpc: a Geant4 tasking application for HEP (LHC/CMS)
#-----------------------------------------------------------------------------#
add_executable(g4hpc
  g4hpc/g4hpc.cc
  g4hpc/src/ActionInitialization.cc
  g4hpc/src/CalorimeterHit.cc
  g4hpc/src/CalorimeterSD.cc
  g4hpc/src/Configuration.cc
  g4hpc/src/DetectorConstruction.cc
  g4hpc/src/DeviceAction.cc
  g4hpc/src/DeviceManager.cc
  g4hpc/src/EmStandardNoMsc.cc
  g4hpc/src/EventAction.cc
  g4hpc/src/GeneratorAction.cc
  g4hpc/src/MagneticField.cc
  g4hpc/src/PhysicsList.cc
  g4hpc/src/RunAction.cc
  g4hpc/src/TrackerHit.cc
  g4hpc/src/TrackerSD.cc
  g4hpc/src/TrackingAction.cc
  temp/LDemoIO.cc
  temp/Transporter.cc
)

target_include_directories(g4hpc PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/g4hpc/include
  ${CMAKE_CURRENT_LIST_DIR}/temp
)

celeritas_target_link_libraries(g4hpc PUBLIC
  Celeritas::Core
  Celeritas::IO
  ${Geant4_LIBRARIES}
)
