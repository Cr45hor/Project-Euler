#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

int const N = 1e8;

int spf[N];

void sieve() {
    for (int x = 1; x < N; x++) {
        spf[x] = x;
    }
    for (int x = 4; x < N; x += 2) {
        spf[x] = 2;
    }
    for (int x = 3; x * x < N; x += 2) {
        if (spf[x] == x) {
            for (int y = x * x; y < N; y += x * 2) {
                if (spf[y] == y) {
                    spf[y] = x;
                }
            }
        }
    }
}

int pow(int a, int n, int mod) {
    int res = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = (int64_t)res * a % mod;
        }
        a = (int64_t)a * a % mod;
    }
    return res;
}

int prime_factor[8];

int factor(int n) {
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        prime_factor[cnt++] = p;
        while (n % p == 0) {
            n /= p;
        }
    }
    return cnt;
}

bool primitive_root(int p, int g) {
    assert(p < N && spf[p] == p);

    int l = factor(p - 1);
    for (int i = 0; i < l; i++) {
        int q = prime_factor[i];
        if (pow(g, (p - 1) / q, p) == 1) {
            return false;
        }
    }
    return true;
}

int tonelli_shanks(int n, int p) {
    int s = 0;
    int q = p - 1;
    while ((q & 1) == 0) { 
        q /= 2; 
        ++s; 
    }
    if (s == 1) {
        int r = pow(n, (p + 1) / 4, p);
        if ((int64_t)r * r % p == n) return r;
        return 0;
    }
    // Find the first quadratic non-residue z by brute-force search
    int z = 1;
    while (pow(++z, (p - 1) / 2, p) != p - 1);
    int c = pow(z, q, p);
    int r = pow(n, (q + 1) / 2, p);
    int t = pow(n, q, p);
    int m = s;
    while (t != 1) {
        int tt = t;
        int i = 0;
        while (tt != 1) {
            tt = (int64_t)tt * tt % p;
            ++i;
            if (i == m) return 0;
        }
        int b = c;
        for (int j = 0; j < m - i - 1; ++j) {
            b = (int64_t)b * b % p;
        }
        int b2 = (int64_t)b * b % p;
        r = (int64_t)r * b % p;
        t = (int64_t)t * b2 % p;
        c = b2;
        m = i;
    }
    if ((int64_t)r * r % p == n) return r;
    return 0;
}

bool check(int p) {
    assert(p < N && spf[p] == p);
    if (p == 5) {
        return true;
    }
    if (p % 5 != 1 && p % 5 != 4) {
        return false;
    }

    int r1 = tonelli_shanks(5, p);
    int r2 = p - r1;
    assert((r1 & 1) == !(r2 & 1));
    assert(r1 > 0 && r2 > 0);
    
    if (r1 & 1) {
        swap(r1, r2);
    }
    assert(r2 & 1);
    int x1 = (int64_t)(r1 + 1) * ((p + 1) / 2) % p;
    int x2 = (r2 + 1) / 2;

    assert(((int64_t)x1 * x1 - x1 - 1) % p == 0);
    assert(((int64_t)x2 * x2 - x2 - 1) % p == 0);
    return primitive_root(p, x1) || primitive_root(p, x2);
}

int main() {
    sieve();

    int64_t res = 0;
    int cnt = 0;
    for (int p = 2; p < N; p++) {
        if (spf[p] == p && check(p)) {
            res += p;
            cnt++;
        }
    }
    cout << cnt << ' ' << res << '\n';
}