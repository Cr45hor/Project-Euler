#include <iostream>
#include <cstdint>
#include <cassert>
#include <bitset>

using namespace std;

int const N = 1e8;

bitset<N> is_prime;

void sieve() {
    is_prime.set();

    is_prime[0] = is_prime[1] = false;
    for (int i = 4; i < N; i += 2) {
        is_prime[i] = false;
    }

    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i * 2) {
                is_prime[j] = false;
            }
        }
    }
}

int normalize(int x, int p) {
    x %= p;
    if (x < 0) {
        x += p;
    }
    return x;
}

int add(int a, int b, int p) {
    return normalize(a + b, p);
}
int mul(int a, int b, int p) {
    return (int64_t)a * b % p;
}

int binpow(int a, int n, int p) {
    // p is guaranteed to be prime
    int res = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = mul(res, a, p);
        }
        a = mul(a, a, p);
    }
    return res;
}

int inv(int a, int p) {
    // p is guaranteed to be prime
    return binpow(a, p - 2, p);
}

int S(int p) {
    int res = 0;
    int n = p - 2;
    int fact = 1;
    for (int k = 3; k <= 5; k++) {
        fact = mul(fact, inv(n, p), p);
        n--;
        res = add(res, fact, p);
    }
    return res;
}

int main() {
    sieve();

    int64_t res = 0;
    for (int p = 5; p < N; p++) {
        if (is_prime[p]) {
            res += S(p);
        }
    }
    cout << res << '\n';

    return 0;
}