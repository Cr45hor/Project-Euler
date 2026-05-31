#include <iostream>
#include <cstdint>

#include "Z.hpp"
int MOD = 1e9 + 9;

int const N = 1e8;

long long v(int p, int n) {
    // Calculate v(p, n!) instead of v(p, n)
    long long k = 0;
    while(n >= p) {
        n /= p;
        k += n;
    }
    return k;
}

int is_prime[N];

void sieve() {
    for (int i = 1; i < N; i += 2) {
        is_prime[i] = 1;
    }
    is_prime[1] = 0;
    is_prime[2] = 1;

    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i) {
                is_prime[j] = 0;
            }
        }
    }
}

int main() {
    sieve();

    Z ans(1);
    for (int p = 2; p < N; p++) {
        if (is_prime[p]) {
            ans *= Z(p).pow(v(p, N) * 2) + 1;
        }
    }
    std::cout << ans << '\n';
}