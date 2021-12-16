#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/baseline_var_carriers.tsv', sep='\t', index_col=False, header=0)

for n in [1, 5, 10]:
    plt.cla()
    df_a = df[df['lbtType'] == "A"]
    df_b = df[df['lbtType'] == "B"]
    for delta in [0, 4, 8]:
        data = df_a[(df_a['numEnbs'] == n) & (df_a['txLock'] == delta)].groupby('numCarriers')['throughputPerCell']\
            .agg([np.mean])\
            .reset_index()
        error = df_a[(df_a['numEnbs'] == n) & (df_a['txLock'] == delta)].groupby('numCarriers')['throughputPerCellStDev'] \
            .agg([np.mean]) \
            .reset_index()
        plt.errorbar(data['numCarriers'], data['mean'], error['mean'], label=f'A D={delta}', marker='s')

    for rng_primary in [0, 1]:
        data = df_b[(df_b['numEnbs'] == n) &
                    (df_b['randomPrimaryCarrier'] == rng_primary)].groupby('numCarriers')['throughputPerCell'] \
            .agg([np.mean]) \
            .reset_index()
        error = df_b[(df_b['numEnbs'] == n) &
                     (df_b['randomPrimaryCarrier'] == rng_primary)].groupby('numCarriers')['throughputPerCellStDev'] \
            .agg([np.mean]) \
            .reset_index()
        plt.errorbar(data['numCarriers'], data['mean'], error['mean'], label=f'B (random primary={rng_primary == 1})', marker='s')

    plt.grid(True)
    plt.legend(loc='best')
    plt.xlabel('Number of carriers')
    plt.ylabel('Throughput per cell, Mbps')
    plt.title(f'N={n}')
    plt.savefig(f'../plots/pdf/baseline_throughput_{n}.pdf')
    plt.savefig(f'../plots/png/baseline_throughput_{n}.png')
    #plt.xlim(1, 10)
    plt.show()
