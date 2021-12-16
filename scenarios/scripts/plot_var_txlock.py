#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/var_txlock.tsv', sep='\t', index_col=False, header=0)
df2 = pd.read_csv('../../../tmp/thesis/scenarios/results/var_txlock.tsv', sep='\t', index_col=False, header=0)

nodes = [1, 2, 5, 10]
num_carriers = 5

for n in nodes:
    data_n = df[(df['numEnbs'] == n)]
    data_n2 = df2[(df2['numEnbs'] == n)]
    plt.cla()
    for num_carriers in {20}:
        T = 75 * num_carriers / n
        data = data_n[(data_n['numCarriers'] == num_carriers)].groupby('txLock')
        t = data['throughputPerCell']\
            .agg([np.mean])\
            .reset_index()
        s = data['throughputPerCellStDev'] \
            .agg([np.mean]) \
            .reset_index()
        plt.errorbar(t['txLock'], t['mean'] / T, s['mean'] / T, marker='s', label=f'AI Z={num_carriers}')
    plt.grid(True)
    plt.legend(loc='best')
    plt.xlabel('D, backoff slots')
    plt.ylabel('Relative throughput per cell')
    plt.title(f'N={n}')
    plt.savefig(f'../plots/pdf/var_txlock_{n}.pdf')
    plt.savefig(f'../plots/png/var_txlock_{n}.png')
    plt.show()

    # plt.cla()
    # for num_carriers in {20}:
    #     data = data_n[(data_n['numCarriers'] == num_carriers)].groupby('txLock')
    #     t = data['channelUsage'] \
    #          .agg([np.mean]) \
    #          .reset_index()
    #     plt.plot(t['txLock'], t['mean'] * 100, marker='s', label=f'Z={num_carriers}')
    # plt.grid(True)
    # plt.legend(loc='best')
    # plt.xlabel('D, backoff slots')
    # plt.ylabel('Channel usage, %')
    # plt.title(f'N={n}')
    # plt.show()

    # plt.cla()
    # for num_carriers in {20}:
    #     data = data_n[(data_n['numCarriers'] == num_carriers)].groupby('txLock')
    #     t = data['successRatio'] \
    #         .agg([np.mean]) \
    #         .reset_index()
    #     plt.plot(t['txLock'], 1 - t['mean'], marker='s', label=f'Z={num_carriers}')
    # plt.grid(True)
    # plt.legend(loc='best')
    # plt.xlabel('D, backoff slots')
    # plt.ylabel('Collision probability')
    # plt.title(f'N={n}')
    # plt.savefig(f'../plots/png/var_txlock_{n}_collision.png')
    # plt.show()
    #plt.savefig(f'../plots/pdf/var_txlock_{n}.pdf')
    #plt.savefig(f'../plots/png/var_txlock_{n}.png')

# plt.cla()
# for n in nodes:
#     data = df[(df['numEnbs'] == n)].groupby('txLock')
#     t = data['channelUsage'] \
#         .agg([np.mean]) \
#         .reset_index()
#     plt.plot(t['txLock'], t['mean'] * 100, marker='s', label=f'N={n}')
#     plt.grid(True)
#     plt.legend(loc='best')
#     plt.xlabel('D, backoff slots')
#     plt.ylabel('Channel usage, %')
#     #plt.savefig(f'../plots/pdf/var_txlock_{n}.pdf')
#     #plt.savefig(f'../plots/png/var_txlock_{n}.png')
# plt.show()
#
# plt.cla()
# for n in nodes:
#     data = df[(df['numEnbs'] == n)].groupby('txLock')
#     t = data['successRatio'] \
#         .agg([np.mean]) \
#         .reset_index()
#     plt.plot(t['txLock'], t['mean'], marker='s', label=f'N={n}')
#     plt.grid(True)
#     plt.legend(loc='best')
#     plt.xlabel('D, backoff slots')
#     plt.ylabel('Success probability')
#     #plt.savefig(f'../plots/pdf/var_txlock_{n}.pdf')
#     #plt.savefig(f'../plots/png/var_txlock_{n}.png')
# plt.show()
