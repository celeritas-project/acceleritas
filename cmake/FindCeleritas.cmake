#----------------------------------*-CMake-*----------------------------------#
# Copyright 2022 UT-Battelle, LLC and other Celeritas developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#[=======================================================================[.rst:

FindCeleritas
-------------

Find the Celeritas library.

#]=======================================================================]

find_package(Celeritas QUIET CONFIG)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Celeritas CONFIG_MODE)

#-----------------------------------------------------------------------------#
