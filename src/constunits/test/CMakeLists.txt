#-----------------------------*-cmake-*----------------------------------------#
# file   constunits/test/CMakeLists.txt
# author Ben R. Ryan <brryan@lanl.gov>
# date   2019 Nov 20
# brief  Generate build project files for constunits/test.
# note   Copyright (C) 2016-2019, Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
project( constunits_test CXX )

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #

file( GLOB test_sources *.cc )

# ---------------------------------------------------------------------------- #
# Build Unit tests
# ---------------------------------------------------------------------------- #

add_scalar_tests(
   SOURCES "${test_sources}"
   DEPS    "Lib_constunits" )

#------------------------------------------------------------------------------#
# End constunits/test/CMakeLists.txt
#------------------------------------------------------------------------------#
