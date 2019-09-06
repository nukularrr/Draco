#!/bin/bash -l

# export dry_run=1

export steps="config build test"
cmd="sbatch -J rel_${package:0:3}-${buildflavor: -10}-$rttversion $access_queue \
-t 1:00:00 -N 1 \
-o $source_prefix/logs/release-$buildflavor-$rttversion.log \
$draco_script_dir/release.msub"
echo -e "\nConfigure, Build and Test $buildflavor-$rttversion version of $package."
echo "$cmd"

jobid=`eval ${cmd} &`
sleep 1m

# trim extra whitespace from number
jobid=`echo ${jobid//[^0-9]/}`
export jobids="$jobid $jobids"
