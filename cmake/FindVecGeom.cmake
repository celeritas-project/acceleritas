#----------------------------------*-CMake-*----------------------------------#
# Copyright 2022 UT-Battelle, LLC and other Celeritas developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#[=======================================================================[.rst:

FindVecGeom
-----------

Find the VecGeom library.

#]=======================================================================]

find_package(VecGeom QUIET CONFIG)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VecGeom CONFIG_MODE)

#-----------------------------------------------------------------------------#
