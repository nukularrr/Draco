#!/bin/bash -l
# ------------------------------------------------------------------------------------------------ #
# File  : ./.gitlab/ci/gitlab-nightly-regress.sh
# Date  : Tuesday, Jun 02, 2020, 10:31 am
# Author: Kelly Thompson <kgt@lanl.gov>
# Note  : Copyright (C) 2020-2021 Triad National Security, LLC., All rights reserved.
# ------------------------------------------------------------------------------------------------ #

# preliminaries and environment
set -e
# shellcheck source=.gitlab/ci/common.sh
source "${JAYENNE_SOURCE_DIR}/.gitlab/ci/common.sh"
# shellcheck source=.gitlab/ci/environments.sh
source "${JAYENNE_SOURCE_DIR}/.gitlab/ci/environments.sh"

if [[ $(which lscpu | grep -c lscpu) -gt 0 ]]; then
  # run "lscpu"
  # NPROC=`lscpu | grep CPU\(s\) | head -n 1 | awk '{ print $2 }'`
  NPROC=$(grep -c processor < /proc/cpuinfo)
fi

[[ -z "${CTEST_NPROC}" ]] && CTEST_NPROC=$NPROC || echo "limiting CTEST_NPROC = $CTEST_NPROC"
[[ -z "${MAXLOAD}" ]] && MAXLOAD=$NPROC || echo "limiting MAXLOAD = $MAXLOAD"

echo -e "\n========== printenv ==========\n"
[[ -z "${SLURM_NODELIST}" ]] || echo "SLURM_NODELIST = ${SLURM_NODELIST}"
echo "HOSTNAME       = ${HOSTNAME}"
echo -e "NPROC       = ${NPROC}\n"
echo -e "CTEST_NPROC = ${CTEST_NPROC}\n"
echo -e "MAXLOAD     = ${MAXLOAD}\n"
echo -e "EXTRA_CMAKE_ARGS = ${EXTRA_CMAKE_ARGS}\n"
echo -e "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}\n"
echo -e "EXTRA_CTEST_ARGS = ${EXTRA_CTEST_ARGS}\n"
if [[ "${AUTODOC}" == "ON" ]]; then
  run "which doxygen"
  run "which latex"
  run "which pdflatex"
  echo "TEXINPUTS = ${TEXINPUTS}"
fi
if [[ "${EXTRA_CTEST_ARGS}" =~ memcheck ]]; then
  run "which valgrind"
fi
printenv >& environment.log
run "pwd"

[[ "${EXTRA_CMAKE_ARGS}" =~ "CODE_COVERAGE" ]] && CODECOV=ON || CODECOV=OFF

#--------------------------------------------------------------------------------------------------#
# Setup compiler flags
#--------------------------------------------------------------------------------------------------#

for i in C CXX Fortran CUDA; do

  # Turn build warnings into fatal errors
  if [[ "${DRACO_ENV}" =~ "xl16" ]]; then
    eval export ${i}_FLAGS+=\" -qhalt=w\"
  else
    if [[ "${i}" == "Fortran" ]] && [[ "${FC}" =~ "ifort" ]]; then
      # do not add the flag
      echo "Skip adding -Werror to ${i}_FLAGS (FC = ${FC})"
    elif [[ "${i}" == "CUDA" ]]; then
      eval export ${i}_FLAGS+=\" -Werror all-warnings\"
    else
      # Works for gcc, llvm, and icpc (not ifort).
      eval export ${i}_FLAGS+=\" -Werror\"
    fi
  fi

  # Enable coverage for Debug builds
  if [[ ${CMAKE_BUILD_TYPE} == Debug ]] && [[ "${CODECOV}" == "ON" ]] ; then
    eval export ${i}_FLAGS+=\" --coverage\"
  fi
done
echo "C_FLAGS       = $C_FLAGS"
echo "CXX_FLAGS     = $CXX_FLAGS"
# shellcheck disable=2154
echo "Fortran_FLAGS = $Fortran_FLAGS"

# Tweak MPI settings
# export OMPI_MCA_btl=self,sm
export OMPI_MCA_btl=^openib

#--------------------------------------------------------------------------------------------------#
# Draco (build and install libraries only, no tests)
#--------------------------------------------------------------------------------------------------#

echo -e "\n========== Draco ==========\n"
if ! [[ -d "${DRACO_BINARY_DIR}" ]]; then run "mkdir -p ${DRACO_BINARY_DIR}"; fi
if ! [[ -d "${DRACO_SOURCE_DIR}" ]]; then
  # https://stackoverflow.com/questions/53391229/clone-another-gitlab-repository-in-gitlab-ci-script
  run "cd ${CI_PROJECT_DIR} && git clone https://gitlab-ci-token:${CI_JOB_TOKEN}@asc-git.lanl.gov/draco/draco.git"
else
  run "cd ${DRACO_SOURCE_DIR} && git pull"
fi
run "cd ${DRACO_BINARY_DIR}"
echo -e "\n========"
run "cmake ${EXTRA_CMAKE_ARGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${CI_PROJECT_DIR}/install ${DRACO_SOURCE_DIR}"

echo " "
run "make -j -l $MAXLOAD"
# don't echo all of the install commands to the terminal.
# shellcheck disable=2086
echo "make -j -l $MAXLOAD install"
make -j -l "$MAXLOAD" install &> /dev/null

if [[ "${AUTODOC}" == "ON" ]]; then
  run "make -j autodoc"
fi

#--------------------------------------------------------------------------------------------------#
# Build and run the tests for jayenne; post results to CDash.
#--------------------------------------------------------------------------------------------------#

echo "To rerun manually, cd to CI directory (pwd), set these variables, run ctest."
echo " "
run "pwd"
echo " "
echo "ARCH               = ${ARCH}"
echo "AUTODOCDIR         = ${AUTODOCDIR}"
echo "BUILD_FLAGS        = ${BUILD_FLAGS}"
echo "CI_PROJECT_DIR     = ${CI_PROJECT_DIR}"
echo "CMAKE_BUILD_TYPE   = ${CMAKE_BUILD_TYPE}"
echo "CTEST_BUILD_NAME   = ${CTEST_BUILD_NAME}"
echo "CTEST_MODE         = ${CTEST_MODE}"
echo "CTEST_NPROC        = ${CTEST_NPROC}"
echo "EXTRA_CMAKE_ARGS   = ${EXTRA_CMAKE_ARGS}"
echo "JAYENNE_BINARY_DIR = ${JAYENNE_BINARY_DIR}"
echo "JAYENNE_SOURCE_DIR = ${JAYENNE_SOURCE_DIR}"
echo "PROJECT            = ${PROJECT}"
echo "SITE_ID            = ${SITE_ID}"
echo "TEST_EXCLUSIONS    = ${TEST_EXCLUSIONS}"
echo " "
echo "CMAKE_TOOLCHAIN_FILE     = ${CMAKE_TOOLCHAINFILE}"
echo "COVERAGE_CONFIGURATION   = ${COVERAGE_CONFIGURATION}"
echo "MEMCHECK_COMMAND_OPTIONS = ${MEMCHECK_COMMAND_OPTIONS}"
echo "MEMCHECK_CONFIGURATION   = ${MEMCHECK_CONFIGURATION}"
echo "MEMORYCHECK_TYPE         = ${MEMORYCHECK_TYPE}"
echo " "
run "ctest -S ${JAYENNE_SOURCE_DIR}/.gitlab/ci/jayenne-nightly.cmake"

[[ "${AUTODOC}" == "ON" ]] && cp ${JAYENNE_SOURCE_DIR}/.gitlab/ci/index.html ${AUTODOCDIR}/.

echo -e "\n======== end .gitlab-nightly-regress.sh ==========\n"

#--------------------------------------------------------------------------------------------------#
# End .gitlab/ci/gitlab-nightly-regress.sh
#--------------------------------------------------------------------------------------------------#
