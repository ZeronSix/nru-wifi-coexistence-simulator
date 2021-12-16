#!/usr/bin/env bash

output=../results/scenario4.tsv
executable=../bin/scenario4

runs=$(seq 1 10)
num_carriers="1 3 5 6 7 8 9 10 11 12 13 16 20 24 28 32"
num_enbs="5 10"
num_wifi_stas="0 1"
lbt_type="A1"
primary_type="Fixed RoundRobin"
tx_lock="0"
leakage_width="8 16 32"
sim_time=60

rm -f $output
echo $'run\tsimTime\tnumCarriers\tnumEnbs\tlbtType\tnumWifiStas\tprimary\tlaaThroughput\twifiThroughput\tD\tleakageWidth' >> $output

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType={5} --txLock={6} --leakageWidth={7} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas ::: $lbt_type ::: $tx_lock ::: $leakage_width

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType=B --primaryType=RoundRobin >> $output"\
 ::: $(seq 1 20) ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas
