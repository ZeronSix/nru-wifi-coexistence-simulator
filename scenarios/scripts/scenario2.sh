#!/usr/bin/env bash

output=../results/scenario2.tsv
executable=../bin/scenario2

runs=$(seq 1 5)
num_carriers=32
num_enbs="1 5 10"
num_wifi_carriers="1 8 16 32"
lbt_type="A1 A2"
tx_locks=$(seq 1 2 24)
sim_time=60

rm -f $output
echo $'run\tsimTime\tnumCarriers\tnumEnbs\tlbtType\tnumWifiCarriers\tD\tsmart\tthroughput' >> $output

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiCarriers={4} --lbtType=B >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_carriers
parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiCarriers={4} --lbtType={5} --txLock={6}>> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_carriers ::: $lbt_type ::: $tx_locks
parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiCarriers={4} --lbtType={5} --txLock={6} --smart >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_carriers ::: $lbt_type ::: $tx_locks
