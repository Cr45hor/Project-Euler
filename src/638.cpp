#include <iostream>
#include <assert.h>

using namespace std;

using ll = long long;

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

int const MOD = 1e9 + 7;
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

Z binom(int a, int b) {
    Z res = 1;
    for (int i = 1; i <= b; i++) {
        res *= Z(a - i + 1) / Z(i);
    }
    return res;
}

Z C(int a, int b, int k) {
    if (k == 1) {
        return binom(a + b, b);
    }

    int m = a + b, r = b;
    Z power_x = 1;
    Z num(1), den(1);
    for (int i = 1; i <= m; i++) {
        power_x *= Z(k);
        Z factor = Z(1) - power_x;
        if (i <= r) {
            den *= factor;
        }
        if (i >= m - r + 1) {
            num *= factor;
        }
    }
    return num / den;
}

int main() {
    Z ans = 0;
    int power_10 = 1;
    for (int k = 1; k <= 7; k++) {
        power_10 *= 10;
        ans += C(power_10 + k, power_10 + k, k);
    }
    cout << ans << '\n';
}