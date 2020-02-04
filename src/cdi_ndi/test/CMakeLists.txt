#-----------------------------*-cmake-*----------------------------------------#
# file   cdi_ndi/test/CMakeLists.txt
# author Ben Ryan <brryan@lanl.gov>
# date   2020 Feb 4
# brief  Generate build project files for cdi_ncdi/test.
# note   Copyright (C) 2016-2020, Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
project( cdi_ndi_test CXX )

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #

set( test_sources
   ${PROJECT_SOURCE_DIR}/tstNDI.cc
)

# ---------------------------------------------------------------------------- #
# Build Unit tests
# ---------------------------------------------------------------------------- #

add_scalar_tests(
   SOURCES "${test_sources}"
   DEPS    "Lib_cdi_ndi"
   )

include( ApplicationUnitTest )

# Run the QueryEospac executable reading interactive commands directly from the
# file QueryEospacInput.dat.  Save the output to QueryEospacInput.out and
# compare this to the gold file.
#add_app_unit_test(
#  DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/tQueryEospac.cmake
#  APP    $<TARGET_FILE_DIR:Exe_QueryEospac>/$<TARGET_FILE_NAME:Exe_QueryEospac>
#  STDINFILE ${PROJECT_SOURCE_DIR}/QueryEospacInput.dat
#  GOLDFILE  ${PROJECT_SOURCE_DIR}/QueryEospac.gold
#)

# Run the QueryEospac executable with options '--version' and '--help' to ensure
# these options print the expected output.
#add_app_unit_test(
#  DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/tQueryEospac.cmake
#  APP    $<TARGET_FILE_DIR:Exe_QueryEospac>/$<TARGET_FILE_NAME:Exe_QueryEospac>
#  TEST_ARGS "--version;--help"
#)

# ---------------------------------------------------------------------------- #
# End cdi_ndi/test/CMakeLists.txt
# ---------------------------------------------------------------------------- #
