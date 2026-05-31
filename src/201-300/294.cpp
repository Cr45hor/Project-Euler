#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>

#include "../Z.hpp"
const int MOD = 1e9;

using namespace std;

int const MAX_SUM = 23;
int const MAX_REM = 23;
int const MAT_SIZE = (MAX_SUM + 1) * MAX_REM;

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

int64_t const N = 3138428376721; // 11^12

int mapping(int sum, int rem) {
    return sum * MAX_REM + rem;
}

Matrix<Z> T;

void pre_construct() {
    for (int sum = 0; sum <= MAX_SUM; sum++) {
        for (int rem = 0; rem < MAX_REM; rem++) {
            int u = mapping(sum, rem);
            for (int d = 0; d <= 9; d++) {
                int nsum = sum + d;
                int nrem = (rem * 10 + d) % MAX_REM;
                if (nsum <= MAX_SUM) {
                    int v = mapping(nsum, nrem);
                    T(u, v) += 1;
                }
            }
        }
    }
}

Z S(int64_t n) {
    Matrix<Z> mat_res = T.pow(n);
    return mat_res(mapping(0, 0), mapping(MAX_SUM, 0));
}

int main() {
    pre_construct();
    cout << S(N) << '\n';
    return 0;
}