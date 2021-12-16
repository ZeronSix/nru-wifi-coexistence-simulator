#!/usr/bin/env python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('../results/scenario1.tsv', sep='\t', index_col=False, header=0)
bianchi_df = pd.read_csv('../results/bianchi.tsv', sep='\t', index_col=False, header=0)

num_enbs = [1, 2, 5, 7, 10]
carriers = [1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 24, 32]
num_wifi_stas = [0, 1, 2]

for N in num_enbs:
    for NW in num_wifi_stas:
        plt.cla()

        T = 'numEnbs'
        X = 'numCarriers'

        data = df[(df[T] == N) & (df['numWifiStas'] == NW)]
        lbt_a1_nosd = data[(data['lbtType'] == 'A1') & (data['D'] == 0)].groupby(X).agg([np.mean]).reset_index()
        lbt_a2_nosd = data[(data['lbtType'] == 'A2') & (data['D'] == 0)].groupby(X).agg([np.mean]).reset_index()
        lbt_a1_sd = data[(data['lbtType'] == 'A1') & (data['D'] == 10)].groupby(X).agg([np.mean]).reset_index()
        lbt_a2_sd = data[(data['lbtType'] == 'A2') & (data['D'] == 10)].groupby(X).agg([np.mean]).reset_index()
        lbt_b_f = data[(data['lbtType'] == 'B') & (data['primary'] == 'fixed')].groupby(X).agg([np.mean]).reset_index()
        lbt_b_rr = data[(data['lbtType'] == 'B') & (data['primary'] == 'roundrobin')].groupby(X).agg([np.mean]).reset_index()
        lbt_b_rand = data[(data['lbtType'] == 'B') & (data['primary'] == 'random')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_f = data[(data['lbtType'] == 'B2') & (data['primary'] == 'fixed')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_rr = data[(data['lbtType'] == 'B2') & (data['primary'] == 'roundrobin')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_rand = data[(data['lbtType'] == 'B2') & (data['primary'] == 'random')].groupby(X).agg([np.mean]).reset_index()

        max_throughput = N ##lbt_a1_nosd[X] # lbt_a1[X] # lbt_a1['numCarriers'] * 75 * N
        # if N == 5 and NW == 0:
        #     print(list(lbt_a1_nosd[X]))
        #     print(list(lbt_a1_nosd['laaThroughput']['mean'] / max_throughput))
        #     print(list(lbt_a2_nosd['laaThroughput']['mean'] / max_throughput))
        #     print(list(lbt_a2_sd['laaThroughput']['mean'] / max_throughput))
        #     print(list(lbt_a2_sd['laaThroughput']['mean'] / max_throughput))
        #     print(list(lbt_b_rr['laaThroughput']['mean'] / max_throughput))
        plt.plot(lbt_a1_nosd[X], lbt_a1_nosd['laaThroughput']['mean'] / max_throughput, label=f'A1 без SD', marker='^')
        plt.plot(lbt_a2_nosd[X], lbt_a2_nosd['laaThroughput']['mean'] / max_throughput, label=f'A2 без SD', marker='*')
        plt.plot(lbt_a1_sd[X], lbt_a1_sd['laaThroughput']['mean'] / max_throughput, label=f'A1 с SD', marker='v')
        plt.plot(lbt_a2_sd[X], lbt_a2_sd['laaThroughput']['mean'] / max_throughput, label=f'A2 с SD', marker='o')
        # plt.plot(lbt_b_f[X], lbt_b_f['laaThroughput']['mean'] / max_throughput, label=f'B1 Fixed', marker='d', linestyle='--', color='r')
        plt.plot(lbt_b_rr[X], lbt_b_rr['laaThroughput']['mean'] / max_throughput, label=f'B1 Even', marker='x', linestyle='-.')
        # plt.plot(lbt_b_rand[X], lbt_b_rand['laaThroughput']['mean'] / max_throughput, label=f'B1 Random', marker='|', linestyle=':', linewidth=2, color='r')

        # plt.plot(lbt_b2_f[X], lbt_b2_f['laaThroughput']['mean'] / max_throughput, label=f'B2 Fixed', marker='d', linestyle='--', color='b')
        plt.plot(lbt_b2_rr[X], lbt_b2_rr['laaThroughput']['mean'] / max_throughput, label=f'B2 Even', marker='|', linestyle='-.')
        # plt.plot(lbt_b2_rand[X], lbt_b2_rand['laaThroughput']['mean'] / max_throughput, label=f'B2 Random', marker='|', linestyle=':', linewidth=2, color='b')

        plt.grid(True)
        plt.legend(loc='best')
        # plt.xlabel('Number of channels')
        # plt.ylabel('NR-U throughput per gNB, Mbps')
        plt.xlabel('Число каналов')
        plt.ylabel('Пропускная способность NR-U в расчете на gNB, Мбит/с')
        plt.tight_layout()
        plt.savefig(f'../plots/pdf/scenario1_nru_Z_{N}_{NW}.pdf')
        plt.savefig(f'../plots/png/scenario1_nru_Z_{N}_{NW}.png')
        plt.show()

for N in num_enbs:
    for NW in num_wifi_stas:
        plt.cla()

        T = 'numEnbs'
        X = 'numCarriers'

        data = df[(df[T] == N) & (df['numWifiStas'] == NW)]
        lbt_a1_nosd = data[(data['lbtType'] == 'A1') & (data['D'] == 0)].groupby(X).agg([np.mean]).reset_index()
        lbt_a2_nosd = data[(data['lbtType'] == 'A2') & (data['D'] == 0)].groupby(X).agg([np.mean]).reset_index()
        lbt_a1_sd = data[(data['lbtType'] == 'A1') & (data['D'] == 10)].groupby(X).agg([np.mean]).reset_index()
        lbt_a2_sd = data[(data['lbtType'] == 'A2') & (data['D'] == 10)].groupby(X).agg([np.mean]).reset_index()
        lbt_b1_f = data[(data['lbtType'] == 'B') & (data['primary'] == 'fixed')].groupby(X).agg([np.mean]).reset_index()
        lbt_b1_rr = data[(data['lbtType'] == 'B') & (data['primary'] == 'roundrobin')].groupby(X).agg([np.mean]).reset_index()
        lbt_b1_rand = data[(data['lbtType'] == 'B') & (data['primary'] == 'random')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_f = data[(data['lbtType'] == 'B2') & (data['primary'] == 'fixed')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_rr = data[(data['lbtType'] == 'B2') & (data['primary'] == 'roundrobin')].groupby(X).agg([np.mean]).reset_index()
        lbt_b2_rand = data[(data['lbtType'] == 'B2') & (data['primary'] == 'random')].groupby(X).agg([np.mean]).reset_index()

        baseline = lbt_a1_nosd[X] * 75 / 100.0

        plt.plot(lbt_a1_nosd[X], (lbt_a1_nosd['laaThroughput']['mean'] + lbt_a1_nosd['wifiThroughput']['mean']) / baseline, label=f'A1 без SD', marker='^')
        plt.plot(lbt_a2_nosd[X], (lbt_a2_nosd['laaThroughput']['mean'] + lbt_a2_nosd['wifiThroughput']['mean']) / baseline, label=f'A2 без SD', marker='*')
        plt.plot(lbt_a1_sd[X], (lbt_a1_sd['laaThroughput']['mean'] + lbt_a1_sd['wifiThroughput']['mean']) / baseline, label=f'A1 с SD', marker='v')
        plt.plot(lbt_a2_sd[X], (lbt_a2_sd['laaThroughput']['mean'] + lbt_a2_sd['wifiThroughput']['mean']) / baseline, label=f'A2 c SD', marker='o')
        #plt.plot(lbt_b1_f[X], (lbt_b1_f['laaThroughput']['mean'] + lbt_b1_f['wifiThroughput']['mean']) / baseline, label=f'B1 Fixed', marker='d', linestyle='--')
        plt.plot(lbt_b1_rr[X], (lbt_b1_rr['laaThroughput']['mean'] + lbt_b1_rr['wifiThroughput']['mean']) / baseline, label=f'B1 Even', marker='x', linestyle='-.')
        #plt.plot(lbt_b1_rand[X], (lbt_b1_rand['laaThroughput']['mean'] + lbt_b1_rand['wifiThroughput']['mean']) / baseline, label=f'B1 Random', marker='|', linestyle=':', linewidth=2)
        #plt.plot(lbt_b2_f[X], (lbt_b2_f['laaThroughput']['mean'] + lbt_b2_f['wifiThroughput']['mean']) / baseline, label=f'B2 Fixed', marker='d', linestyle='--')
        plt.plot(lbt_b2_rr[X], (lbt_b2_rr['laaThroughput']['mean'] + lbt_b2_rr['wifiThroughput']['mean']) / baseline, label=f'B2 Even', marker='|', linestyle='-.')
        #plt.plot(lbt_b2_rand[X], (lbt_b2_rand['laaThroughput']['mean'] + lbt_b2_rand['wifiThroughput']['mean']) / baseline, label=f'B2 Random', marker='x', linestyle=':', linewidth=2)

        plt.grid(True)
        plt.legend(loc='best')
        # plt.xlabel('Number of channels')
        # plt.ylabel('Channel utilization, %')
        plt.xlabel('Число каналов')
        plt.ylabel('Коэффициент использования, %')
        plt.tight_layout()
        plt.savefig(f'../plots/pdf/scenario1_total_Z_{N}_{NW}.pdf')
        plt.savefig(f'../plots/png/scenario1_total_Z_{N}_{NW}.png')
        plt.show()

