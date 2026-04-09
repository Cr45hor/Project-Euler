#include <iostream>
#include <algorithm>
#include <assert.h>
#include <stdint.h>
#include <array>

using namespace std;

int normalize(int x, int p) {
    if (x >= p) {
        x -= p;
    }
    else if (x < 0) {
        x += p;
    }
    return x;
}

int binpow(int a, int64_t n, int p) {
    int res = 1, base = a % p;
    for (; n; n >>= 1) {
        if (n & 1) 
            res = 1LL * res * base % p;
        base = 1LL * base * base % p;
    }
    return res;
}

int MOD = 1e9 + 7;
struct Z{
    int x;
    Z(int x = 0): x(normalize(x, MOD)) {}
    Z(int64_t x): x(normalize(x % MOD, MOD)) {}

    Z operator-() const {
        return Z(-x);
    }
    Z inv() const {
        // Assume MOD is prime
        return Z(binpow(x, MOD - 2, MOD));
    }

    Z operator+(const Z& other) const {
        return Z(x + other.x);
    }
    Z operator-(const Z& other) const {
        return Z(x - other.x);
    }
    Z operator*(const Z& other) const {
        return Z(1LL * x * other.x);
    }
    Z operator/(const Z& other) const {
       return *this * other.inv();
    }

    Z& operator+=(const Z& other) {
        return *this = *this + other;
    }
    Z& operator-=(const Z& other) {
        return *this = *this - other;
    }
    Z& operator*=(const Z& other) {
        return *this = *this * other;
    }
    Z& operator/=(const Z& other) {
        return *this = *this / other;
    }

    bool operator==(const Z& other) const {
        return x == other.x;
    }

    friend ostream& operator<<(ostream& os, const Z& z) {
        return os << z.x;
    }
    friend istream& operator>>(istream& is, Z& z) {
        int64_t x;
        is >> x;
        z = Z(x);
        return is;
    }
};

int64_t N = (int64_t)1e16 + 1;

Z sum_square(int64_t n) {
    if (n < 0) {
        return Z(0);
    }
    int64_t factor[3] = {n, 2 * n + 1, n + 1};
    int r2 = n % 2;
    int r3 = n % 3;
    factor[r2 * 2] >>= 1;
    factor[r3] /= 3;
    return Z(factor[0]) * Z(factor[1]) * Z(factor[2]);
}

Z sum_square(int64_t L, int64_t R) {
    return sum_square(R) - sum_square(L - 1);
}

Z sum(int64_t L, int64_t R) {
    int64_t factor[2] = {R - L + 1, L + R};
    int r2 = factor[0] % 2;
    factor[r2] >>= 1;
    return Z(factor[0]) * Z(factor[1]);
}

int64_t L[64];
int64_t length[64];

array<Z, 6> S(int64_t l, int64_t len, int popcnt) {
    /* [n^2 * c_n^2.
        n^2 * c_n,
        n * c_n^2,
        n * c_n,
        c_n^2,
        c_n] 
    */

    if (len == 1) {
        Z ll = Z(l) * l;
        array<Z, 6> res =  {ll * Z(popcnt) * Z(popcnt),
                            ll * Z(popcnt), 
                            Z(l) * Z(popcnt) * Z(popcnt), 
                            Z(l) * Z(popcnt), 
                            Z(popcnt) * Z(popcnt),
                            Z(popcnt)};
        return res;
    }

    int64_t mid = l + (len >> 1);
    // int64_t r = l + len;
    // [l, l + 2^k) -> [l, l + 2^(k - 1)) + [l + 2^(k - 1), l + 2^k)
    auto res_l = S(l, len >> 1, popcnt);
    Z offset = Z(len >> 1);
    Z offset2 = offset * offset;

    array<Z, 6> res_r;

    res_r[0] = res_l[0] + res_l[1] * 2 + sum_square(l, mid - 1) + res_l[2] * offset * 2 + res_l[3] * offset * 4 + sum(l, mid - 1) * offset * 2 + (res_l[4] + res_l[5] * 2 + offset) * offset2;
    res_r[1] = res_l[1] + res_l[3] * offset * 2 + res_l[5] * offset2 + sum_square(l, mid - 1) + sum(l, mid - 1) * offset * 2 + offset2 * offset;
    res_r[2] = res_l[2] + res_l[3] * 2 + sum(l, mid - 1) + res_l[4] * offset + res_l[5] * offset * 2 + offset2;
    res_r[3] = res_l[3] + res_l[5] * offset + sum(l, mid - 1) + offset2;
    res_r[4] = res_l[4] + offset + (res_l[5] * 2);
    res_r[5] = res_l[5] + offset;

    array<Z, 6> res;
    for (int i = 0; i < 6; i++) {
        res[i] = res_l[i] + res_r[i];
    }
    return res;
}

Z S(int64_t n) {
    // Sum of n^2 * popcnt(n)^2 for n = 0 to N-1
    Z res = 0;
    for (int i = 0; i < 64 && n > 0; i++) {
        int k = 63 - __builtin_clzll(n);
        length[i] = 1LL << k;
        L[i + 1] = L[i] + length[i];
        n -= length[i];
        array<Z, 6> ses_seg = S(L[i], length[i], i);
        res += ses_seg[0];
    }

    return res;
}

int main() {
    cout << S(N) << endl;
    return 0;
}