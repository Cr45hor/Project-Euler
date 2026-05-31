#include <iostream>
#include <cstdint>
#include <cassert>

#include "Z.hpp"
int MOD = 1e9 + 7;

int const MAX_A = 1e7;
int64_t N = 1e12;

Z fact[MAX_A + 2];
Z inv_fact[MAX_A + 2];

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
    for (int i = 1; i <= MAX_A + 1; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    inv_fact[MAX_A + 1] = fact[MAX_A + 1].inv();
    for (int i = MAX_A; i >= 1; --i) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1);
    }
}

Z S(int m, int64_t n) {
    Z result = 0;
    Z A_next = 1; 

    for (int l = m - 1; l >= 0; l--) {
        Z sign_term = ((m - l) % 2 == 1) ? (Z(0) - C(m + 1, l + 1)) : C(m + 1, l + 1);
        Z A_l = A_next * 2 + sign_term;
        result += geometric_sum(Z(l), n) * (Z(1) - A_l);
        A_next = A_l; 
    }
    
    return result;
}

int main() {
    precompute();
    std::cout << S(MAX_A, N) << '\n';
}