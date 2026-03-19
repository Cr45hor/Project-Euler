#include <iostream>
#include <assert.h>
#include <numeric>

using namespace std;

using ll = long long;

int const MAX_N = 6e4;
int const N = 5e4;

int normalize(int x, int p) {
    if (x >= p) {
        x -= p;
    }
    else if (x < 0) {
        x += p;
    }
    return x;
}

int binpow(int a, ll n, int p) {
    int res = 1, base = a % p;
    for (; n; n >>= 1) {
        if (n & 1) 
            res = 1LL * res * base % p;
        base = 1LL * base * base % p;
    }
    return res;
}

int const MOD = 999676999;
struct Z{
    int x;
    Z(int x = 0): x(normalize(x, MOD)) {}
    Z(ll x): x(normalize(x % MOD, MOD)) {}

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
        ll x;
        is >> x;
        z = Z(x);
        return is;
    }
};

int spf[MAX_N];
void sieve() {
    spf[1] = 1;
    for (int i = 2; i < MAX_N; i++) {
        if (!spf[i]) {
            spf[i] = i;
            if (1LL * i * i >= MAX_N) {
                continue;
            }
            for (int j = i * i; j < MAX_N; j += i) {
                if (!spf[j]) {
                    spf[j] = i;
                }
            }
        }
    }
}

int D[MAX_N];
Z G[MAX_N];

int main() {
    sieve();

    D[1] = 1;
    for (int x = 2; x < MAX_N; x++) {
        int p = spf[x];
        // cout << "spf[" << x << "] = " << p << endl;
        if (x == p) {
            D[x] = 1;
        }
        else {
            D[x] = (x / p) + p * D[x / p];
        }
        // cout << "D[" << x << "] = " << D[x] << endl;
    }

    G[0] = 1;
    for (int x = 1; x < MAX_N; x++) {
        for (int s = x; s < MAX_N; s++) {
            G[s] += G[s - x] * D[x];
        }
    }
    cout << accumulate(G + 1, G + N + 1, Z(0)) << '\n';

    return 0;
}