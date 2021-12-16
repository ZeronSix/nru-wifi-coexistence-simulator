#!/usr/bin/env bash

output=../results/scenario3.tsv
executable=../bin/scenario3

runs=$(seq 1 5)
num_carriers=32
num_enbs="1 5 10"
lbt_type="A1 A2"
tx_locks=$(seq 1 2 24)
wifi_load="0.25 0.5 0.75 1"
wifi_t_on_ms="1 10 100 500"
sim_time=60
pt="Fixed RoundRobin"

rm -f $output
echo $'run\tsimTime\tnumCarriers\tnumEnbs\tlbtType\tD\twifiLoad\twifiTOnMs\tprimary\tthroughput' >> $output

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --lbtType=B --wifiLoad={4} --wifiTOnMs={5} --primaryType={6} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $wifi_load ::: $wifi_t_on_ms ::: $pt

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --lbtType=B --wifiLoad={4} --wifiTOnMs={5} --primaryType=Random >> $output"\
 ::: $(seq 1 100) ::: $num_carriers ::: $num_enbs ::: $wifi_load ::: $wifi_t_on_ms

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --lbtType={4} --txLock={5} --wifiLoad={6} --wifiTOnMs={7} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $lbt_type ::: $tx_locks ::: $wifi_load ::: $wifi_t_on_ms
