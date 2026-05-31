#include <iostream>
#include <cmath>
#include <cassert>

#include "../Z.hpp"
int const MOD = 1e9 + 7;

using namespace std;

int const N = 1e8;
int64_t const M = 1e16;
int const multiplier = M / N;

Z fact[N + 1];
Z inv_fact[N + 1];
Z pow_multiplier = Z(2).pow(multiplier);

void precompute() {
    assert(M % N == 0);

    fact[0] = 1;
    for (int i = 1; i <= N; i++) {
        fact[i] = fact[i - 1] * i;
    }
    inv_fact[N] = fact[N].inv();
    for (int i = N - 1; i >= 0; i--) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1);
    }
}

int main() {
    precompute();

    int min_c1 = (N & 1);
    Z power = pow_multiplier.pow(min_c1);
    Z ans = 0;

    for (int c1 = min_c1; c1 <= N; c1 += 2) {
        int c2 = (N - c1) >> 1;
        ans += (fact[N] * inv_fact[c1] * inv_fact[c2] * inv_fact[c2]) * power;
        power *= pow_multiplier.pow(2);
    }

    cout << ans << '\n';
}