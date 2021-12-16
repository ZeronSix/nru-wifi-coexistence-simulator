#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/scenario2.tsv', sep='\t', index_col=False, header=0)

num_enbs = [1, 5, 10]
num_wifi_carriers = [1, 8, 16, 32]

for N in num_enbs:
    for ZW in num_wifi_carriers:
        plt.cla()
        data = df[(df['numEnbs'] == N) & (df['numWifiCarriers'] == ZW)]
        lbt_a1_smart = data[(data['lbtType'] == 'A1') & (data['smart'] == 'smart')].groupby('D').agg([np.mean]).reset_index()
        lbt_a2_smart = data[(data['lbtType'] == 'A2') & (data['smart'] == 'smart')].groupby('D').agg([np.mean]).reset_index()
        lbt_a1_basic = data[(data['lbtType'] == 'A1') & (data['smart'] == 'basic')].groupby('D').agg([np.mean]).reset_index()
        lbt_a2_basic = data[(data['lbtType'] == 'A2') & (data['smart'] == 'basic')].groupby('D').agg([np.mean]).reset_index()
        lbt_b = data[data['lbtType'] == 'B'].groupby('D').agg([np.mean]).reset_index()
        #lbt_b

        max_throughput = 75 * 32
        plt.plot(lbt_a1_smart['D'], lbt_a1_smart['throughput']['mean'] / max_throughput, label='A1 smart', marker='s')
        plt.plot(lbt_a2_smart['D'], lbt_a2_smart['throughput']['mean'] / max_throughput, label='A2 smart', marker='s')
        plt.plot(lbt_a1_basic['D'], lbt_a1_basic['throughput']['mean'] / max_throughput, label='A1 basic', marker='s')
        plt.plot(lbt_a2_basic['D'], lbt_a2_basic['throughput']['mean'] / max_throughput, label='A2 basic', marker='s')
        plt.axhline(y=lbt_b['throughput']['mean'].item() / max_throughput, label='B', marker='s', color='y')

        plt.grid(True)
        plt.legend(loc='best')
        plt.xlabel('D')
        plt.ylabel('Relative throughput per cell')
        plt.title(f'{N} gNBs, ZW={ZW}')
        plt.savefig(f'../plots/pdf/scenario2_{N}_{ZW}.pdf')
        plt.savefig(f'../plots/png/scenario2_{N}_{ZW}.png')
        plt.show()
