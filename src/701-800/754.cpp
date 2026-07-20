#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../Z.hpp"
int const MOD = 1e9 + 7;

using namespace std;

int const N = 1e8;

int spf[N + 1];
vector<int> primes;

void sieve() {
    for (int i = 1; i <= N; ++i) {
        spf[i] = i;
    }
    for (int i = 4; i <= N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i <= N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
    for (int i = 2; i <= N; ++i) {
        if (spf[i] == i) {
            primes.push_back(i);
        }
    }
}

Z fact[N + 1];
Z inv_fact[N + 1];

void precompute() {
    sieve();

    fact[0] = 1;
    for (int i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * i;
    }

    inv_fact[N] = Z(fact[N]).inv().x;
    for (int i = N - 1; i >= 0; --i) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1);
    }
}

Z fast_inv(int n) {
    assert(n >= 1 && n <= N);
    return inv_fact[n] * fact[n - 1];
}

vector<int> factors(int n) {
    assert(n >= 1 && n <= N);
    vector<int> res;
    res.reserve(8);
    while(n > 1) {
        int p = spf[n];
        res.push_back(p);
        while (n % p == 0) {
            n /= p;
        }
    }
    return res;
}

Z h[N + 1];

void construct_rhs() {
    h[1] = 1;
    for (int n = 2; n <= N; n++) {
        h[n] = fact[n];
        for (int p : factors(n)) {
            Z inv_p = fast_inv(p);
            int expo = n;
            while(expo % p == 0) {
                expo /= p;
                h[n] *= inv_p.pow(expo);
            }
        }
    }
}

Z g[N + 1];

int main() {
    ios_base::sync_with_stdio(false);

    precompute();
    construct_rhs();

    for (int i = 1; i <= N; i++) {
        g[i] = h[i];
    }

    for (int n = 1; n <= N; n++) {
        for (int m = n * 2; m <= N; m += n) {
            g[m] /= g[n];
        }
    }

    cout << accumulate(g + 1, g + N + 1, Z(1), multiplies<Z>()) << "\n";
}