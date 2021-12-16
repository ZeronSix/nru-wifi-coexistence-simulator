#!/usr/bin/env bash

output=../results/var_wifi_cw.tsv
executable=../bin/baseline

runs=$(seq 1 5)
sim_time=20
num_carriers=20
num_enbs="1 2 5 10"
tx_lock="$(seq 1 3 25)"
wifi_cwmin="15 31 63 127"

rm -f $output
./table_header.sh >> $output

# for run in $(seq 1 $runs); do
#   for num_enbs in 1 2 5 10; do #2 5 10; do
#     for wifi_carriers in 0; do # 5 10 15 20; do
#       for tx_lock in $(seq 1 2 19); do # $(seq 0 4 20) $(seq 24 4 40); do
#         $executable --run=$run --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs=$num_enbs --txLock=$tx_lock --numWifiCarriers=$wifi_carriers >> $output
#         $executable --run=$run --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs=$num_enbs --txLock=$tx_lock --numWifiCarriers=$wifi_carriers --smart>> $output
#       done
#     done
#   done
# done

parallel --eta "$executable --run={1} --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs={2} --txLock={3} --numWifiCarriers=$num_carriers --wifiCwMin={4} >> $output" ::: $runs ::: $num_enbs ::: $tx_lock ::: $wifi_cwmin
#parallel --eta "$executable --run={1} --simTime=$sim_time --numWifiStas=1 --numCarriers=$num_carriers --numEnbs={2} --txLock={3} --numWifiCarriers={4} --smart >> $output" ::: $runs ::: $num_enbs ::: $tx_lock ::: $wifi_carriers
