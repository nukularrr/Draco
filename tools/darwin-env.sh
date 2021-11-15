#!/bin/bash

# ------------------------------------------------------------------------------------------------ #
# Darwin Environment setups
# ------------------------------------------------------------------------------------------------ #

if [[ $(uname -n) =~ "darwin-fe" ]]; then
  echo "FATAL ERROR: This script must be run from a back-end node" && exit 1
fi
if ! [[ $ddir ]] ;then
  echo "FATAL ERROR: Expected ddir to be set in the environment. (darwin-env.sh)" && exit 1
fi

export VENDOR_DIR=/projects/draco/vendors
DRACO_ARCH=$(/usr/projects/draco/vendors/bin/target_arch)

# Extra cmake options
export CONFIG_BASE+=" -DCMAKE_VERBOSE_MAKEFILE=ON"

export DRACO_ARCH

# ------------------------------------------------------------------------------------------------ #
# End
# ------------------------------------------------------------------------------------------------ #
