#include <iostream>
#include <algorithm>
#include <cstring>
#include <assert.h>
#include <numeric>

using namespace std;

using u64 = long long;
using u128 = __uint128_t;

int const MAX = 5e7 + 1;
int const RANGE = 5e7 + 1;

u64 t(int n) {
    return 2LL * n * n - 1;
}

int spf[MAX];
int list_prime[MAX];
int num_prime = 0;

void sieve() {
    spf[1] = 1;
    for (int x = 2; x < MAX; x++) {
        if (spf[x] == 0) {
            list_prime[num_prime++] = x;
            for (int y = x; y < MAX; y += x) {
                if (spf[y] == 0) {
                    spf[y] = x;
                }
            }
        }
    } 
}

int valid[RANGE];
u64 value_tn[RANGE];

u64 binpower(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1)
            result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

/* Takes as input an odd prime p and n < p and returns r
 * such that r * r = n [mod p]. */
long tonelli_shanks(long n, long p) {
    long s = 0;
    long q = p - 1;
    while ((q & 1) == 0) { q /= 2; ++s; }
    if (s == 1) {
        long r = binpower(n, (p+1)/4, p);
        if ((r * r) % p == n) return r;
        return 0;
    }
    // Find the first quadratic non-residue z by brute-force search
    long z = 1;
    while (binpower(++z, (p-1)/2, p) != p - 1);
    long c = binpower(z, q, p);
    long r = binpower(n, (q+1)/2, p);
    long t = binpower(n, q, p);
    long m = s;
    while (t != 1) {
        long tt = t;
        long i = 0;
        while (tt != 1) {
            tt = (tt * tt) % p;
            ++i;
            if (i == m) return 0;
        }
        long b = binpower(c, binpower(2, m-i-1, p-1), p);
        long b2 = (b * b) % p;
        r = (r * b) % p;
        t = (t * b2) % p;
        c = b2;
        m = i;
    }
    if ((r * r) % p == n) return r;
    return 0;
}

void update(int p, int r) {
    for (int i = r; i < RANGE; i += p) {
        valid[i] = 0;
        while(value_tn[i] % p == 0) {
            value_tn[i] /= p;
        }
    }
    if (t(r) == 1LL * p) {
        valid[r] = 1; 
    }
}

bool check_composite(u64 n, u64 a, u64 d, int s) {
    u64 x = binpower(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int r = 1; r < s; r++) {
        x = (u128)x * x % n;
        if (x == n - 1)
            return false;
    }
    return true;
};

bool MillerRabin(u64 n, int iter = 30) { // returns true if n is probably prime, else returns false.
    if (n < 4)
        return n == 2 || n == 3;

    int s = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    for (int i = 0; i < iter; i++) {
        int a = 2 + rand() % (n - 3);
        if (check_composite(n, a, d, s))
            return false;
    }
    return true;
}

int main() {
    sieve();

    for (int i = 0; i < RANGE; i++) {
        valid[i] = 1;
        value_tn[i] = t(i);
    }
    valid[0] = valid[1] = 0;

    for (int i = 1; i < num_prime; i++) {
        int p = list_prime[i];
        if (p % 8 != 1 && p % 8 != 7) {
            continue;
        }
        if (p >= RANGE) {
            break;
        }

        int n = (p + 1)/2;
        int r = tonelli_shanks(n, p);
        int r1 = r, r2 = p - r;
        // cout << p << ' ' << r1 << ' ' << r2 << '\n';
        update(p, r);
        update(p, p - r);
    }

    for (int n = 2; n < RANGE; n++) {
        if (value_tn[n] == 1) {
            continue;
        }
        else {
            valid[n] = (value_tn[n] == t(n)) && MillerRabin(value_tn[n]);
            // if (valid[n]) {
            //     cout << "Additional " << n << '\n';
            // }
        }
    }

    int number_prime_tn = accumulate(valid, valid + RANGE, 0LL);
    // int not_clear = accumulate(value_tn, value_tn + RANGE, 0LL, [&](u64 sum, u64 x) { return sum + (x > 1); });
    cout << number_prime_tn << '\n';
    //  << ' ' << not_clear << '\n';

    return 0;
}