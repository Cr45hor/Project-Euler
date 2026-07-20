#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>

#include "../Z.hpp"
const int MOD = 1e9 + 9;

using namespace std;

int const MAT_SIZE = 2;

template<typename T>
struct Matrix {
    T mat[MAT_SIZE * MAT_SIZE]{};

    Matrix(T id = T(0)) {
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                mat[i * MAT_SIZE + j] = (i == j ? id : T(0));
            }
        }
    }

    Matrix(T other[MAT_SIZE][MAT_SIZE]) : mat(MAT_SIZE * MAT_SIZE, T(0)) {
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int j = 0; j < MAT_SIZE; j++) {
                mat[i * MAT_SIZE + j] = other[i][j];
            }
        }
    }

    T& operator()(int i, int j) {
        return mat[i * MAT_SIZE + j];
    }
    
    const T& operator()(int i, int j) const {
        return mat[i * MAT_SIZE + j];
    }

    Matrix operator+(const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < MAT_SIZE * MAT_SIZE; i++) {
            res.mat[i] = mat[i] + other.mat[i];
        }
        return res;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < MAT_SIZE; i++) {
            for (int k = 0; k < MAT_SIZE; k++) {
                // Minor optimization: skip if mat(i,k) is 0
                if (mat[i * MAT_SIZE + k] == T(0)) continue; 
                
                for (int j = 0; j < MAT_SIZE; j++) {
                    res.mat[i * MAT_SIZE + j] += mat[i * MAT_SIZE + k] * other.mat[k * MAT_SIZE + j];
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

Z compute_seq(int u_0, int u_1, int64_t n) {
    if (n == 0) {
        return Z(u_0);
    } 
    else if (n == 1) {
        return Z(u_1);
    }

    Matrix<Z> M;
    M(0, 0) = M(0, 1) = M(1, 0) = 1;

    Matrix<Z> Mn = M.pow(n - 1);
    return Mn(0, 0) * Z(u_1) + Mn(0, 1) * Z(u_0);
}

pair<Z, Z> P(int64_t n) {
    Z a = Z(2).pow(n) * compute_seq(0, 1, n - 1) - (n & 1 ? -1 : 1);
    Z b = Z(4).pow(n) - Z(2).pow(n) * compute_seq(2, 1, n) + (n & 1 ? -1 : 1);
    return {a, b};
}

const int64_t N = 1e18;

int main() {
    auto [a, b] = P(N);
    cout << (a * b.inv()) << '\n';
    return 0;
}