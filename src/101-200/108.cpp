#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

int const N = 1e8;
int const THRESHOLD = 4e6;

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

int F[N];
int G[N];

int main() {
    sieve();

    F[1] = 0;
    for (int n = 2; n < N; n++) {
        int p = spf[n];
        int q = n;
        while(q % p == 0) {
            q /= p;
        }
        F[n] = F[q] + 1;
    }

    for (int n = 1; n < N; n++) {
        G[n] = 1;
    }

    for (int d = 2; d < N; d++) {
        for (int g = 1; d * g < N; g++) {
            G[d * g] += 1 << (F[d] - 1);
        }
    }

    // cout << find_if(G, G + N, [](int x) { return x > THRESHOLD; }) - G << '\n';
    cout << *max_element(G, G + N) << '\n';
}