#!/usr/bin/env bash

output=../results/baseline_var_carriers.tsv
executable=../bin/baseline

runs=5
sim_time=360
max_carriers=10

rm -f $output
./table_header.sh >> $output

for run in $(seq 1 $runs); do
  for num_enbs in 1 5 10; do
    for num_carriers in $(seq 1 $max_carriers) 20; do
      for tx_lock in 0 4 8; do
        $executable --run=$run --simTime=$sim_time --numCarriers=$num_carriers --numEnbs=$num_enbs --txLock=$tx_lock >> $output
      done
      $executable --run=$run --simTime=$sim_time --numCarriers=$num_carriers --numEnbs=$num_enbs --lbtType=B >> $output
      $executable --run=$run --simTime=$sim_time --numCarriers=$num_carriers --numEnbs=$num_enbs --lbtType=B --randomPrimaryCarrier >> $output
    done
  done
done


