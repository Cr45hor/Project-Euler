#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

int const N = 2e7;

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

int I(int n) {
    int buf[8];
    int L = factorize(n, buf);
    assert(L > 0 && L <= 8);

    int prod[1 << 8];
    prod[0] = 1;
    for (int mask = 1; mask < (1 << L); mask++) {
        int msb = 31 - __builtin_clz(mask);
        prod[mask] = prod[mask ^ (1 << msb)] * buf[msb];
    }

    bool pow2 = !(n & 1);
    int sol_2[4]{};
    int num_sol_2 = 0;
    if (pow2) {
        if (buf[0] == 2) {
            // 2^1
            // 1 solution: 1 (mod 2)
            sol_2[num_sol_2++] = 1;
        }
        else if (buf[0] == 4) {
            // 2^2
            // 2 solutions: 1 (mod 4) and 3 (mod 4)
            sol_2[num_sol_2++] = 1;
            sol_2[num_sol_2++] = 3;
        }
        else {
            // 2^k, k >= 3
            // 4 solutions: 1 (mod 2^k), 2^(k-1) - 1 (mod 2^k), 2^(k-1) + 1 (mod 2^k), 2^k - 1 (mod 2^k)
            sol_2[num_sol_2++] = 1;
            sol_2[num_sol_2++] = buf[0] / 2 - 1;
            sol_2[num_sol_2++] = buf[0] / 2 + 1;
            sol_2[num_sol_2++] = buf[0] - 1;
        }
    }

    int full_mask = (1 << L) - 1;
    if (pow2) {
        assert(buf[0] % 2 == 0);
        full_mask ^= 1;
    }

    int res = 1;
    for (int submask = 0; submask < 1 << (L - pow2); submask++) {
        int mask = submask << pow2;
        int p = prod[mask];
        int q = prod[full_mask ^ mask];
        // Solve x = 1 mod p and x = -1 mod q
        // Equivalent to compute x = q * (q^-1 mod p) - p * (p^-1 mod q) mod n
        // It is guaranteed that p and q are coprime, so q^-1 mod p and p^-1 mod q exist
        int sol = 0;
        if (p == 1) {
            sol = 1;
        }
        else if (q == 1) {
            sol = n - 1;
        }
        else {
            int64_t x, y;
            int g = extended_gcd(p, q, x, y);
            assert(g == 1 && 1LL * p * x + 1LL * q * y == 1);
            int inv_p = normalize(x, q), inv_q = normalize(y, p);
            sol = normalize((int64_t)q * inv_q - (int64_t)p * inv_p, n);
        }

        // Finally, solve x = sol mod u and x = sol_2[i] mod v where u = p * q = n/2^k and v = 2^k
        int u = p * q, v = buf[0];
        int sol_final[5]{};
        for (int i = 0; i < num_sol_2; i++) {
            int64_t x, y;
            int g = extended_gcd(u, v, x, y);
            assert(g == 1 && 1LL * u * x + 1LL * v * y == 1);
            int inv_n2 = normalize(x, v), inv_2k = normalize(y, u);
            sol_final[i] = normalize(((int64_t)(u) * inv_n2 * sol_2[i] + (int64_t)v * inv_2k * sol) % (int64_t)n, n);
        }

        sol_final[4] = sol;
        int l = (pow2 ? 0 : 4);
        int r = (pow2 ? num_sol_2 - 1 : 4);
        for (int i = l; i <= r; i++) {
            if (sol_final[i] < n - 1) {
                res = max(res, sol_final[i]);
            }
        }
    }
    return res;
}

int main() {
    sieve();

    assert(I(15) == 11);
    assert(I(100) == 51);
    assert(I(7) == 1);

    int64_t res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int n = 3; n <= N; n++) {
        res += I(n);
    }
    cout << res << '\n';
    return 0;
}