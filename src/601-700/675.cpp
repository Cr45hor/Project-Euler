#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include "../Z.hpp"
int const MOD = 1e9 + 87;

using namespace std;

int const N = 1e7;

int spf[N + 1];

void sieve() {
    for (int i = 1; i <= N; i += 2) {
        spf[i] = i;
    }
    for (int i = 2; i <= N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i <= N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += 2 * i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int inv[N * 2];

void precompute() {
    sieve();

    inv[1] = 1;
    for (int i = 2; i < N * 2; ++i) {
        inv[i] = (MOD - 1LL * (MOD / i) * inv[MOD % i] % MOD) % MOD;
    }
}

int freq[N];
Z res = 0;
Z prod = 1;

int main() {
    precompute();

    for (int n = 2; n <= N; n++) {
        int x = n;
        while (x > 1) {
            int p = spf[x];
            int k = 0;
            while(x % p == 0) {
                x /= p;
                k++;
            }
            prod *= inv[freq[p] * 2 + 1];
            freq[p] += k;
            prod *= (freq[p] * 2 + 1);
        }
        res += prod;
    }
    cout << res << '\n';

    return 0;
}