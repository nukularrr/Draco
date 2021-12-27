#--------------------------------------------*-cmake-*---------------------------------------------#
# file   .gitlab/ci/draco-nightly.cmake
# author Kelly Thompson <kgt@lanl.gov>
# date   Tuesday, Jun 02, 2020, 11:44 am
# brief  CTest regression script for DRACO
# note   Copyright (C) 2021 Triad National Security, LLC., All rights reserved.
# ------------------------------------------------------------------------------------------------ #
# Ref: http://www.cmake.org/Wiki/CMake_Scripting_Of_CTest

set(CTEST_SOURCE_DIRECTORY "$ENV{DRACO_SOURCE_DIR}")
set(CTEST_BINARY_DIRECTORY "$ENV{DRACO_BINARY_DIR}")
set(CTEST_PROJECT_NAME "$ENV{PROJECT}")

set(CTEST_SUBMIT_URL "https://rtt.lanl.gov/cdash3/submit.php?project=$ENV{PROJECT}")
set(CTEST_CURL_OPTIONS CURLOPT_SSL_VERIFYPEER_OFF CURLOPT_SSL_VERIFYHOST_OFF)
set(CTEST_NIGHTLY_START_TIME "22:00:00 MDT")

if(WIN32)
  set(CTEST_CMAKE_GENERATOR "Visual Studio 16 2019")
else()
  set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
endif()
set(CTEST_SITE "$ENV{SITE_ID}")
if(DEFINED ENV{ARCH})
  string(APPEND CTEST_SITE "-$ENV{ARCH}")
endif()
string(APPEND CTEST_SITE "-gr") # indicating job was initiated by a gitlab runner.

set(CTEST_BUILD_NAME "$ENV{CTEST_BUILD_NAME}")
set(CTEST_MODE "$ENV{CTEST_MODE}")

cmake_host_system_information(RESULT MPI_PHYSICAL_CORES QUERY NUMBER_OF_PHYSICAL_CORES)
set(CMAKE_BUILD_PARALLEL_LEVEL ${MPI_PHYSICAL_CORES})
if(DEFINED ENV{CTEST_NPROC})
  set(CTEST_PARALLEL_LEVEL $ENV{CTEST_NPROC})
else()
  set(CTEST_PARALLEL_LEVEL ${MPI_PHYSICAL_CORES})
endif()
set(CTEST_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
if(CTEST_BUILD_TYPE STREQUAL MemCheck)
  set(CTEST_BUILD_TYPE Debug)
endif()
string(APPEND EXTRA_CMAKE_ARGS " $ENV{EXTRA_CMAKE_ARGS}")

string(TOUPPER ${CTEST_BUILD_TYPE} UPPER_CTEST_BUILD_TYPE)
if(${UPPER_CTEST_BUILD_TYPE} MATCHES DEBUG)
  set(CTEST_BUILD_CONFIGURATION "DEBUG")
elseif(${UPPER_CTEST_BUILD_TYPE} MATCHES RELWITHDEBINFO)
  set(CTEST_BUILD_CONFIGURATION "RELWITHDEBINFO")
elseif(${UPPER_CTEST_BUILD_TYPE} MATCHES MINSIZEREL)
  set(CTEST_BUILD_CONFIGURATION "MINSIZEREL")
elseif(${UPPER_CTEST_BUILD_TYPE} MATCHES RELEASE)
  set(CTEST_BUILD_CONFIGURATION "RELEASE")
else()
  message(FATAL_ERROR, "CTEST_BUILD_TYPE (= ${UPPER_CTEST_BUILD_TYPE}) not set or not valid.")
endif()

file(TO_CMAKE_PATH "${CI_PROJECT_DIR}" CI_PROJECT_DIR)
string(CONCAT CTEST_CONFIGURE_COMMAND "cmake"
              " -DCMAKE_INSTALL_PREFIX=$ENV{CI_PROJECT_DIR}/install" " ${EXTRA_CMAKE_ARGS}")
if(WIN32)
  file(TO_CMAKE_PATH "${CTEST_SOURCE_DIRECTORY}" CTEST_SOURCE_DIRECTORY)
  file(TO_CMAKE_PATH "$ENV{CMAKE_TOOLCHAIN_FILE}" CMAKE_TOOLCHAIN_FILE)
  string(APPEND CTEST_CONFIGURE_COMMAND " -A x64 -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
  string(APPEND CTEST_CONFIGURE_COMMAND " -DDRACO_LIBRARY_TYPE=SHARED")
else()
  string(APPEND CTEST_CONFIGURE_COMMAND " -DCMAKE_BUILD_TYPE=${UPPER_CTEST_BUILD_TYPE}")
endif()
if(DEFINED ENV{AUTODOCDIR})
  string(APPEND CTEST_CONFIGURE_COMMAND " -DAUTODOCDIR=$ENV{AUTODOCDIR}")
endif()
string(APPEND CTEST_CONFIGURE_COMMAND " ${CTEST_SOURCE_DIRECTORY}")
message(
  "
CTEST_PARALLEL_LEVEL = ${CTEST_PARALLEL_LEVEL}
MPI_PHYSICAL_CORES   = ${MPI_PHYSICAL_CORES}

CTEST_CMAKE_GENERATOR     = ${CTEST_CMAKE_GENERATOR}
CTEST_BUILD_CONFIGURATION = ${CTEST_BUILD_CONFIGURATION}
CTEST_CONFIGURE_COMMAND   = ${CTEST_CONFIGURE_COMMAND}
")

set(CTEST_USE_LAUNCHERS FALSE)
set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 50)
set(CTEST_UPDATE_COMMAND "git")
set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "Skipping git update (no-op).")

if(${CTEST_SCRIPT_ARG} MATCHES Configure)
  ctest_empty_binary_directory("${CTEST_BINARY_DIRECTORY}")
  message("ctest_start( ${CTEST_MODE} )")
  ctest_start(${CTEST_MODE})
else()
  message("ctest_start( ${CTEST_MODE} APPEND )")
  ctest_start(${CTEST_MODE} APPEND)
endif()

ctest_update(SOURCE ${CTEST_SOURCE_DIRECTORY})

# ------------------------------------------------------------------------------------------------ #
# Configure the project with cmake.
# ------------------------------------------------------------------------------------------------ #
if(${CTEST_SCRIPT_ARG} MATCHES Configure)

  message(
    "
ctest_configure(
  BUILD        ${CTEST_BINARY_DIRECTORY}
  RETURN_VALUE configure_failure
  CAPTURE_CMAKE_ERROR ctest_configure_errors)
")
  ctest_configure(
    BUILD "${CTEST_BINARY_DIRECTORY}"
    RETURN_VALUE configure_failure
    CAPTURE_CMAKE_ERROR ctest_configure_errors)

  if(configure_failure)
    message("${ctest_configure_errors}")
    ctest_submit()
    message(FATAL_ERROR "configuration error")
  endif()

endif()

# ------------------------------------------------------------------------------------------------ #
# Build it
# ------------------------------------------------------------------------------------------------ #
if(${CTEST_SCRIPT_ARG} MATCHES Build)

  if(NOT WIN32)
    set(CTEST_BUILD_FLAGS "-j ${CTEST_PARALLEL_LEVEL}")
  endif()
  if(DEFINED ENV{AUTODOCDIR})
    # build one unit test
    message(
      "
ctest_build(
  TARGET Ut_dsxx_tstAssert_exe
  FLAGS $ENV{BUILD_FLAGS}
  RETURN_VALUE build_failure
  CAPTURE_CMAKE_ERROR ctest_build_errors)
")
    ctest_build(
      TARGET Ut_dsxx_tstAssert_exe
      FLAGS "$ENV{BUILD_FLAGS}"
      RETURN_VALUE build_failure
      CAPTURE_CMAKE_ERROR ctest_build_errors)
    # build autodoc target.
    message(
      "
ctest_build(
  TARGET autodoc
  FLAGS $ENV{BUILD_FLAGS}
  RETURN_VALUE build_failure
  CAPTURE_CMAKE_ERROR ctest_build_errors)
")
    ctest_build(
      TARGET autodoc
      FLAGS "$ENV{BUILD_FLAGS}"
      RETURN_VALUE build_failure
      CAPTURE_CMAKE_ERROR ctest_build_errors)
    if(build_failure)
      message("${ctest_build_errors}")
      ctest_submit()
      message(FATAL_ERROR "build error")
    endif()
  else()
    message(
      "
ctest_build(
  FLAGS $ENV{BUILD_FLAGS}
  RETURN_VALUE build_failure
  PARALLEL_LEVEL ${CTEST_PARALLEL_LEVEL}
  CAPTURE_CMAKE_ERROR ctest_build_errors)")
    ctest_build(
      FLAGS "$ENV{BUILD_FLAGS}"
      RETURN_VALUE build_failure
      # cmake-3.21 -- PARALLEL_LEVEL ${CMAKE_BUILD_PARALLEL_LEVEL}
      CAPTURE_CMAKE_ERROR ctest_build_errors)

    if(build_failure)
      message("${ctest_build_errors}")
      ctest_submit()
      message(FATAL_ERROR "build error")
    endif()
  endif()

endif()

# ------------------------------------------------------------------------------------------------ #
# Run the tests
# ------------------------------------------------------------------------------------------------ #
if(${CTEST_SCRIPT_ARG} MATCHES Test)

  if(DEFINED ENV{AUTODOCDIR})
    # Run one test to avoid missing data on CDash
    message("
ctest_test( RETURN_VALUE test_failure INCLUDE dsxx_tstAssert)
")
    ctest_test(RETURN_VALUE test_failure INCLUDE dsxx_tstAssert)
  else()
    if(DEFINED ENV{TEST_EXCLUSIONS})
      set(CTEST_TEST_EXTRAS EXCLUDE $ENV{TEST_EXCLUSIONS})
    endif()
    list(APPEND CTEST_TEST_EXTRAS PARALLEL_LEVEL ${CTEST_PARALLEL_LEVEL})
    message("
ctest_test( RETURN_VALUE test_failure ${CTEST_TEST_EXTRAS})
")
    ctest_test(RETURN_VALUE test_failure ${CTEST_TEST_EXTRAS})

    if(DEFINED ENV{COVERAGE_CONFIGURATION})
      find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
      ctest_coverage()
    endif()

    if(DEFINED ENV{MEMCHECK_CONFIGURATION})
      if(DEFINED ENV{MEMORYCHECK_TYPE})
        set(CTEST_MEMORYCHECK_TYPE $ENV{MEMORYCHECK_TYPE})
      else()
        find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
        string(CONCAT CTEST_MEMORYCHECK_COMMAND_OPTIONS
                      "-q --tool=memcheck --trace-children=yes --leak-check=full "
                      "--show-reachable=yes --num-callers=20 --gen-suppressions=all")
        if(ENV{MEMCHECK_COMMAND_OPTIONS})
          string(APPEND CTEST_MEMORYCHECK_COMMAND_OPTIONS " $ENV{MEMCHECK_COMMAND_OPTIONS}")
        endif()
        set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE
            "${CTEST_SOURCE_DIRECTORY}/config/valgrind_suppress.txt")
      endif()

      set(CTEST_TEST_TIMEOUT 1200) # 1200 seconds = 20 minutes per test
      # message("ctest_memcheck(INCLUDE_LABEL memcheck)") ctest_memcheck(INCLUDE_LABEL memcheck)
      message("ctest_memcheck()")
      ctest_memcheck()

    endif()
  endif()

endif()
message("==> Done with testing")

# ------------------------------------------------------------------------------------------------ #
# Submit the results to CDash
# ------------------------------------------------------------------------------------------------ #
if(${CTEST_SCRIPT_ARG} MATCHES Submit)
  message("==> Trying to submit results to CDash")
  ctest_submit()

  if(test_failure)
    message(FATAL_ERROR "test failure")
  endif()

endif()

# ------------------------------------------------------------------------------------------------ #
# end .gitlab/ci/draco-nightly.cmake
# ------------------------------------------------------------------------------------------------ #
