#!/usr/bin/env bash

output=../results/var_wifi_load.tsv
executable=../bin/baseline

runs=5
sim_time=60
num_carriers=20

rm -f $output
./table_header.sh >> $output

for run in $(seq 1 $runs); do
  for num_enbs in 1 2 5 10; do #2 5 10; do
    for wifi_carriers in 5 10 15 20; do
      for tx_lock in $(seq 1 2 19); do # $(seq 0 4 20) $(seq 24 4 40); do
        $executable --run=$run --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs=$num_enbs --txLock=$tx_lock --numWifiCarriers=$wifi_carriers >> $output
        $executable --run=$run --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs=$num_enbs --txLock=$tx_lock --numWifiCarriers=$wifi_carriers --smart>> $output
      done
    done
  done
done


