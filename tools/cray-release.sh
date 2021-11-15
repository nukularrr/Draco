#!/bin/bash

# export dry_run=1
# config and build on front-end

# If not set, then initialize var to empty string.
[[ -z $jobnameext ]] && jobnameext=""
[[ -z $partition ]] && partition=""

echo -e "\nConfigure and build ${package:-unknown} for ${buildflavor:-unknown}-${rttversion:-unknown}."
echo
export steps="config build"
logfile="${source_prefix:-notset}/logs/release-$buildflavor-$rttversion-cb.log"
run "${draco_script_dir:-notset}/release.msub &> $logfile"

# Run the tests on the back-end.
export steps="test"
logfile="$source_prefix/logs/release-$buildflavor-$rttversion-t.log"
cmd="sbatch -J rel-${package:0:3}-${buildflavor: -10}-$rttversion -t 8:00:00 -N 1 \
 --gres=craynetwork:0 -o $logfile $draco_script_dir/release.msub"
echo -e "\nTest $package for $buildflavor-$rttversion."
echo "$cmd"
jobid=$(eval "${cmd}")
sleep 1m
# trim extra whitespace from number
jobid="${jobid//[^0-9]/}"
export jobids="$jobid $jobids"

##---------------------------------------------------------------------------##
## End
##---------------------------------------------------------------------------##
