#include <iostream>
#include <cassert>
#include <cstdint>

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

// MOD should be prime and less than 2^30
const int MOD = 1e9 + 7; 

// Modular integer class for arithmetic modulo MOD
struct Z{
    int x;
    Z(int X = 0): x(normalize(X, MOD)) {}
    Z(int64_t X): x(normalize(X % MOD, MOD)) {}

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
    Z pow(int64_t n) const {
        return Z(binpow(x, n, MOD));
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

    friend std::ostream& operator<<(std::ostream& os, const Z& z) {
        return os << z.x;
    }
    friend std::istream& operator>>(std::istream& is, Z& z) {
        int64_t x;
        is >> x;
        z = Z(x);
        return is;
    }
};

int const MAT_SIZE = 3;

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

    Matrix pow(int64_t n) const {
        Matrix res(1), base = *this;
        for (; n; n >>= 1) {
            if (n & 1) 
                res *= base;
            base *= base;
        }
        return res;
    }
};

Z F(int n, int x) {
    Matrix<Z> mat;
    mat(0, 0) = mat(2, 0) = Z(x);
    mat(0, 1) = mat(2, 1) = Z(1LL * x * x);
    mat(1, 0) = mat(2, 2) = Z(1);

    Matrix<Z> res = mat.pow(n - 1);
    return (res(2, 0) + res(2, 2)) * Z(x);
}

int main() {
    cout << F(7, 11) << '\n';
}