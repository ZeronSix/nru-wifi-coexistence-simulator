#!/usr/bin/env bash

output=../results/bianchi.tsv
executable=../bin/bianchi

runs=$(seq 1 5)
n="$(seq 1 20) 30 50"
sim_time=60

rm -f $output
echo $'run\tsimTime\tnumWifiStas\tthroughput' >> $output

parallel --eta "$executable --run={1} --simTime=$sim_time --numWifiStas={2} >> $output" ::: $runs ::: $n


