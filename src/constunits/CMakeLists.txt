#-----------------------------*-cmake-*----------------------------------------#
# file   constunits/CMakeLists.txt
# author Ben R. Ryan <brryan@lanl.gov>
# date   2019 Nov 20
# brief  Generate build project files for units.
# note   Copyright (C) 2016-2019, Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.9.0)
project( constunits CXX )

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #

file( GLOB sources *.cc )
file( GLOB explicit_instantiations *_pt.cc )
file( GLOB headers *.hh )

# ---------------------------------------------------------------------------- #
# Build package library
# ---------------------------------------------------------------------------- #

add_component_library(
   TARGET       Lib_constunits
   TARGET_DEPS  Lib_dsxx
   LIBRARY_NAME ${PROJECT_NAME}
   HEADERS      "${headers}"
   SOURCES      "${sources}"  )

# ---------------------------------------------------------------------------- #
# Installation instructions
# ---------------------------------------------------------------------------- #

install( TARGETS Lib_constunits EXPORT draco-targets DESTINATION ${DBSCFGDIR}lib )
install( FILES ${headers} DESTINATION ${DBSCFGDIR}include/constunits )

# ---------------------------------------------------------------------------- #
# Unit tests
# ---------------------------------------------------------------------------- #

if( BUILD_TESTING )
   add_subdirectory( test )
endif()

# ---------------------------------------------------------------------------- #
# Autodoc
# ---------------------------------------------------------------------------- #

process_autodoc_pages()

#------------------------------------------------------------------------------#
# End constunits/CMakeLists.txt
#------------------------------------------------------------------------------#
