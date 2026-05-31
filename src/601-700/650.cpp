#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <cstdint>
#include <numeric>

#include "Z.hpp"
int MOD = 1e9 + 7;

using namespace std;

int const N = 2e4;

int spf[N + 1];
int inv[N + 1];

void sieve() {
    for (int x = 1; x <= N; x++) {
        spf[x] = x;
    }

    for (int x = 4; x <= N; x += 2) {
        spf[x] = 2;
    }

    for (int p = 3; p * p <= N; p += 2) {
        if (spf[p] == p) {
            for (int x = p * p; x <= N; x += p) {
                if (spf[x] == x) {
                    spf[x] = p;
                }
            }
        }
    }

    for (int x = 1; x <= N; x++) {
        inv[x] = Z(x).inv().x;
    }
}

Z sum_divisor(int p, int k) {
    assert(p > 1);
    return (Z(p).pow(k + 1) - 1) * inv[p - 1];
}

int expo[N + 1];

Z D(int n) {
    for (int k = 1; k <= n; k++) {
        int x = k;
        while(x > 1) {
            int p = spf[x], d = 0;
            while(x % p == 0) {
                x /= p;
                d++;
            }
            expo[p] += d * (k * 2 - n - 1);
        }
    }

    Z res = 1;
    for (int p = 2; p <= n; p++) {
        if (spf[p] == p) {
            assert(expo[p] >= 0);
            res *= sum_divisor(p, expo[p]);
            expo[p] = 0;
        }
    }
    return res;
}

Z S(int n) {
    Z res = 0;
    for (int k = 1; k <= n; k++) {
        res += D(k);
    }
    return res;
}

int main() {
    sieve();
    cout << S(N) << '\n';
    return 0;
}