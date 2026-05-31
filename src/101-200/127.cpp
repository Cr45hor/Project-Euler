#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdint>

using namespace std;

int const N = 120000;

int spf[N];

void sieve() {
    for (int i = 1; i < N; i += 2) {
        spf[i] = i;
    }
    spf[2] = 2;
    for (int i = 4; i < N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int rad[N];

void precompute() {
    sieve();

    rad[1] = 1;
    for (int n = 2; n < N; n++) {
        int p = spf[n];
        int prev = n / p;
        if (prev % p == 0) {
            rad[n] = rad[prev];
        } else {
            rad[n] = rad[prev] * p;
        }
    }
}

int main() {
    precompute();

    int64_t cnt = 0, sum = 0;
    #pragma omp parallel for reduction(+:cnt) reduction(+:sum)
    for (int c = 1; c < N; c++) {
        int limit = (c - 1) / rad[c];
        if (limit <= 1) {
            continue;
        }
        for (int a = 1; a < c - a; a++) {
            if (1LL * rad[a] * rad[c - a] <= limit && __gcd(a, c) == 1) {
                sum += c;
                cnt++;
            }
        }
    }

    cout << cnt << ' ' << sum << '\n';
}