#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

int const N = 1e7;

using namespace std;

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
            for (int j = i * i; j <= N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int factorize(int n, int buf[8]) {
    assert(n > 0 && n <= N);
    int cnt = 0;
    while(n > 1) {
        int p = spf[n];
        buf[cnt] = 1;
        while(spf[n] == p) {
            n /= p;
            buf[cnt] *= p;
        }
        cnt++;
    }
    return cnt;
}

int normalize(int64_t x, int mod) {
    x %= mod;
    if (x < 0) {
        x += mod;
    }
    return (int)x;
}

int extended_gcd(int a, int b, int64_t &x, int64_t &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t x1, y1;
    int d = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

int M(int n) {
    int buf[8];
    int L = factorize(n, buf);
    assert(L > 0 && L <= 8);

    int full_mask = (1 << L) - 1;
    int prod[1 << 8];
    prod[0] = 1;
    for (int mask = 1; mask <= full_mask; mask++) {
        int msb = 31 - __builtin_clz(mask);
        prod[mask] = prod[mask ^ (1 << msb)] * buf[msb];
    }

    int res = 0;
    for (int mask = 1; mask <= full_mask; mask++) {
        int p = prod[mask];
        int q = prod[full_mask ^ mask];
        // Solve x = 1 mod p and x = 0 mod q
        // Equivalent to compute x = q * (q^-1 mod p) mod n
        // It is guaranteed that p and q are coprime, so q^-1 mod p exists
        int64_t x, y;
        int g = extended_gcd(p, q, x, y);
        assert(g == 1);
        assert(1LL * p * x + 1LL * q * y == 1);
        int inv = normalize(y, p);
        int sol = (int64_t)q * inv % n;
        res = max(res, sol);
    }
    return res;
}

int main() {
    sieve();

    int64_t res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int n = 2; n <= N; n++) {
        res += M(n);
    }
    cout << res << '\n';
    return 0;
}