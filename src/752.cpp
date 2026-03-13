#include <iostream>
#include <assert.h>
#include <numeric>

using namespace std;

using ll = long long;

int const MAT_SIZE = 2;
int const MAX_N = 1e6 + 6;
int const N = 1e6;

template<typename T>
struct Matrix {
    T mat[MAT_SIZE][MAT_SIZE]{};
    Matrix(T id = T(0)) {
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                mat[i][j] = (i == j ? id : T(0));
            }
        }
    }
    Matrix(T other[MAT_SIZE][MAT_SIZE]) {
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                mat[i][j] = other[i][j];
            }
        }
    }

    T& operator()(int i, int j) {
        return mat[i][j];
    }
    const T& operator()(int i, int j) const {
        return mat[i][j];
    }

    Matrix operator+(const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                res(i, j) = mat[i][j] + other(i, j);
            }
        }
        return res;
    }
    Matrix operator*(const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                for (int k = 0; k < MAT_SIZE; k++) {
                    res(i, j) += mat[i][k] * other(k, j);
                }
            }
        }
        return res;
    }

    Matrix& operator+=(const Matrix& other) {
        return *this = *this + other;
    }
    Matrix& operator*=(const Matrix& other) {
        return *this = *this * other;
    }

    Matrix pow(ll n) const {
        Matrix res(1), base = *this;
        for (; n; n >>= 1) {
            if (n & 1) 
                res *= base;
            base *= base;
        }
        return res;
    }
};

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

int MOD;
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

int spf[MAX_N]{};
void sieve() {
    spf[1] = 1;
    for (int x = 2; x < MAX_N; x++) {
        if (spf[x] == 0) {
            spf[x] = x;
            if (1LL * x * x >= MAX_N) {
                continue;
            }
            for (int y = x * x; y < MAX_N; y += x) {
                if (spf[y] == 0) {
                    spf[y] = x;
                }
            }
        }
    }
}

int factorize(int x, int buffer[16]) {
    int idx = 0;
    while(x > 1) {
        int p = spf[x];
        while(x % p == 0) {
            x /= p;
        }
        buffer[idx++] = p;
    }
    return idx;
}

bool check(ll n, int p) {
    Matrix<Z> mat;
    mat(0, 0) = mat(1, 0) = mat(1, 1) = Z(1);
    mat(0, 1) = Z(7);

    Matrix<Z> res = mat.pow(n);
    return res(0, 0) == Z(1) && res(0, 1) == Z(0);
}

ll calculate_g(int p) {
    if (p == 2 || p == 3) {
        return 0;
    }
    else if (p == 7) {
        return 7;
    }

    int factors[32]{};
    int idx1 = factorize(p - 1, factors);
    int idx2 = factorize(p + 1, factors + idx1);
    int num_prime = idx1 + idx2;

    ll res = 1LL * p * p - 1;
    MOD = p;
    for (int i = 0; i < num_prime; i++) {
        int prime = factors[i];
        while(res % prime == 0 && check(res / prime, prime)) {
            res /= prime;
        }
    }
    return res;
}

ll g[MAX_N]{};

int main() {
    sieve();

    for (int x = 2; x < MAX_N; x++) {
        if (spf[x] == x) {
            g[x] = calculate_g(x);
            // cout << "g(" << x << ") = " << g[x] << '\n';
            continue;
        }
        if (!(x & 1) || (x % 3 == 0)) {
            g[x] = 0;
            continue;
        }
        int y = x, p = spf[x];
        while(y % p == 0) {
            y /= p;
        }
        if (y == 1) {
            g[x] = g[p] * (x / p);
        }
        else {
            g[x] = lcm(g[y], g[x / y]);
        }
        // cout << "g(" << x << ") = " << g[x] << '\n';
    }

    ll res = accumulate(g + 2, g + N + 1, 0LL);
    cout << res << '\n';

    return 0;
}