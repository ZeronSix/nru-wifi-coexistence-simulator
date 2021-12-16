#!/usr/bin/env bash

output=../results/scenario1.tsv
executable=../bin/scenario1

runs=$(seq 1 20)
num_carriers="1 3 5 6 7 8 9 10 11 12 13 16 20 24 28 32"
num_enbs="1 2 5 7 10"
num_wifi_stas="0 1 2"
lbt_type="A1 A2"
primary_type="Fixed RoundRobin"
tx_lock="0 10"
sim_time=120

rm -f $output
echo $'run\tsimTime\tnumCarriers\tnumEnbs\tlbtType\tnumWifiStas\tprimary\tlaaThroughput\twifiThroughput\tD' >> $output

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType={5} --txLock={6} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas ::: $lbt_type ::: $tx_lock

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType=B --primaryType=Random >> $output"\
 ::: $(seq 1 100) ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas
parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType=B --primaryType={5} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas ::: $primary_type

parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType=B2 --primaryType=Random >> $output"\
 ::: $(seq 1 50) ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas
parallel --eta "$executable --run={1} --simTime=$sim_time --numCarriers={2} --numEnbs={3} --numWifiStas={4} --lbtType=B2 --primaryType={5} >> $output"\
 ::: $runs ::: $num_carriers ::: $num_enbs ::: $num_wifi_stas ::: $primary_type
