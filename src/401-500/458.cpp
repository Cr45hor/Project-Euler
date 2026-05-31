#include <iostream>
#include <assert.h>
#include <numeric>

using namespace std;

using ll = long long;

int const MAT_SIZE = 7;

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

int const MOD = 1e9;
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

int main() {
    Matrix <Z> T(Z(0));
    T(0, 1) = Z(MAT_SIZE);
    for (int i = 1; i < MAT_SIZE; i++) {
        if (i + 1 < MAT_SIZE) {
            T(i, i + 1) = Z(MAT_SIZE - i);
        }
        for (int j = i; j >= 1; j--) {
            T(i, j) = Z(1);
        }
    }

    long long const N = 1e12;

    Matrix<Z> Tn = T.pow(N);
    Z res = 0;
    for (int i = 1; i < MAT_SIZE; i++) {
        res += Tn(0, i);
    }
    cout << res << endl;

    return 0;
}