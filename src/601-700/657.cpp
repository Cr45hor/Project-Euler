#include <iostream>
#include <cstdint>
#include <cassert>

#include "Z.hpp"
int MOD = 1e9 + 7;

/*
def geometric_sum(a: int, n: int) -> int:
    # Sum of 1 + a + a^2 + ... + a^n
    if a == 1:
        return n + 1
    return (pow(a, n + 1, MOD) - 1) * pow(a - 1, MOD - 2, MOD) % MOD

def I(a: int, n: int) -> int:
    fact = [1] * (a + 1)
    inv_fact = [1] * (a + 1)
    for i in range(2, a + 1):
        fact[i] = fact[i - 1] * i % MOD
    inv_fact[a] = pow(fact[a], MOD - 2, MOD)
    for i in range(a - 1, 0, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD
    inv_fact[0] = 1

    return sum((-1)**(k + 1) * math.comb(a, k) * geometric_sum(a - k, n) for k in range(1, a + 1)) % MOD
*/

int const MAX_A = 1e7;
int64_t N = 1e12;

Z fact[MAX_A + 1];
Z inv_fact[MAX_A + 1];

Z geometric_sum(Z a, int64_t n) {
    if (a.x == 0) {
        return Z(1);
    }
    if (a.x == 1) {
        return Z(n + 1);
    }
    return (a.pow(n + 1) - 1) * fact[a.x - 2] * inv_fact[a.x - 1];
}

Z C(int n, int k) {
    if (k > n) {
        return Z(0);
    }
    return fact[n] * inv_fact[k] * inv_fact[n - k];
}

void precompute() {
    fact[0] = inv_fact[0] = 1;
    for (int i = 1; i <= MAX_A; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    inv_fact[MAX_A] = fact[MAX_A].inv();
    for (int i = MAX_A - 1; i >= 1; --i) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1);
    }
}

Z I(int a, int64_t n) {
    Z result = 0;
    for (int k = 1; k <= a; ++k) {
        result += C(a, k) * geometric_sum(Z(a - k), n) * (k & 1 ? 1 : -1);
    }
    return result;
}

int main() {
    precompute();
    std::cout << I(MAX_A, N) << '\n';
}