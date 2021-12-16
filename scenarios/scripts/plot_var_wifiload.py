#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/var_wifi_load.tsv', sep='\t', index_col=False, header=0)

nodes = [1, 2, 5, 10]
num_carriers = 20

for n in nodes:
    data = df[(df['numEnbs'] == n)]

    plt.cla()
    for wifi_carriers in [5, 10, 15, 20]:
        for scheme in {0, 1}:
            T = 75 * num_carriers / n
            data_ = data[(data['smart'] == scheme) & (data['numWifiCarriers'] == wifi_carriers)].groupby('txLock')
            t = data_['throughputPerCell']\
                .agg([np.mean])\
                .reset_index()
            s = data_['throughputPerCellStDev'] \
                .agg([np.mean]) \
                .reset_index()
            plt.errorbar(t['txLock'], t['mean'] / T, s['mean'] / T, marker='s', label=f'{"smart" if scheme else "basic"} K={wifi_carriers}')
    plt.grid(True)
    plt.legend(loc='best')
    plt.xlabel('D, backoff slots')
    plt.ylabel('Relative throughput per cell')
    plt.title(f'N={n}')
    plt.savefig(f'../plots/pdf/var_wifiload_{n}.pdf')
    plt.savefig(f'../plots/png/var_wifiload_{n}.png')
    plt.show()
