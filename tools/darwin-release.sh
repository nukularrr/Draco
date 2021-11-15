#!/bin/bash -l

# ------------------------------------------------------------------------------------------------ #
# Darwin ARM
# ------------------------------------------------------------------------------------------------ #

[[ -z $package ]] && package=""

if [[ $(uname -n) =~ "darwin-fe" ]]; then
  echo "FATAL ERROR: This script must be run from a back-end node" && exit 1
fi

# export dry_run=1
export steps="config build test"
echo -e "\nConfigure, Build and Test ${buildflavor:-unknown}-${rttversion:-unkown} version of $package.\n"
"${draco_script_dir:-notset}/release.msub"

# ------------------------------------------------------------------------------------------------ #
# End
# ------------------------------------------------------------------------------------------------ #
