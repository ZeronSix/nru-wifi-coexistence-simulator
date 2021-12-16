#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/scenario3.tsv', sep='\t', index_col=False, header=0)

num_enbs = [1, 5, 10]
wifi_load = [0.25, 0.5, 0.75, 1]
wifi_t_on_ms = [1, 10, 100, 500]

for N in num_enbs:
    for load in wifi_load:
        for t in wifi_t_on_ms:
            plt.cla()
            data = df[(df['numEnbs'] == N) & (df['wifiLoad'] == load) & (df['wifiTOnMs'] == t)]
            lbt_a1 = data[(data['lbtType'] == 'A1')].groupby('D').agg([np.mean]).reset_index()
            lbt_a2 = data[(data['lbtType'] == 'A2')].groupby('D').agg([np.mean]).reset_index()
            lbt_b = data[data['lbtType'] == 'B'].groupby('D').agg([np.mean]).reset_index()

            X = 'D'
            lbt_b_f = data[(data['lbtType'] == 'B') & (data['primary'] == 'fixed')].groupby(X).agg([np.mean]).reset_index()
            lbt_b_rr = data[(data['lbtType'] == 'B') & (data['primary'] == 'roundrobin')].groupby(X).agg([np.mean]).reset_index()
            lbt_b_rand = data[(data['lbtType'] == 'B') & (data['primary'] == 'random')].groupby(X).agg([np.mean]).reset_index()

            max_throughput = 75 * 32
            plt.plot(lbt_a1['D'], lbt_a1['throughput']['mean'] / max_throughput, label='A1', marker='s')
            plt.plot(lbt_a2['D'], lbt_a2['throughput']['mean'] / max_throughput, label='A2', marker='s')
            plt.axhline(y=lbt_b_f['throughput']['mean'].item() / max_throughput, label='B fixed', color='y')
            plt.axhline(y=lbt_b_rr['throughput']['mean'].item() / max_throughput, label='B RR', color='g')
            plt.axhline(y=lbt_b_rand['throughput']['mean'].item() / max_throughput, label='B random', color='m')

            plt.grid(True)
            plt.legend(loc='best')
            plt.xlabel('D')
            plt.ylabel('Relative throughput per cell')
            plt.title(f'{N} gNBs, Wi-Fi load={load} T_on={t} ms')
            plt.savefig(f'../plots/pdf/scenario3_{N}_{load}_{t}.pdf')
            plt.savefig(f'../plots/png/scenario3_{N}_{load}_{t}.png')
            plt.show()
