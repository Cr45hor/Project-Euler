#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

using namespace std;

int const N = 2e7;
int const K = 15e6;

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
                spf[j] = i;
            }
        }
    }
}

int freq[N];

void update(int p, int k, int sgn) {
    freq[p] += k * sgn;
}

int v_fact(int p, int n) {
    int res = 0;
    while (n > 0) {
        n /= p;
        res += n;
    }
    return res;
}

int64_t res = 0;

int main() {
    sieve();

    for (int p = 2; p <= N; p++) {
        if (spf[p] == p) {
            freq[p] = v_fact(p, N) - v_fact(p, N - K) - v_fact(p, K);
            res += 1LL * p * freq[p];
        }
    }
    cout << res << '\n';

    return 0;
}