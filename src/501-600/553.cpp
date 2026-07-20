#include <iostream>
#include <cstdint>
#include <cassert>

#include "../Z.hpp"
const int MOD = 1e9 + 7;

using namespace std;

int const N = 10000;
int const M = 10;

Z fact[N + 1];
Z inv_fact[N + 1];

void precompute() {
    fact[0] = Z(1);
    for (int i = 1; i <= N; i++) {
        fact[i] = fact[i - 1] * Z(i);
    }

    inv_fact[N] = fact[N].inv();
    for (int i = N - 1; i >= 0; i--) {
        inv_fact[i] = inv_fact[i + 1] * Z(i + 1);
    }
}

Z comb(int n, int k) {
    if (k < 0 || k > n) {
        return Z(0);
    }
    return fact[n] * inv_fact[k] * inv_fact[n - k];
}

Z A[N + 1];

void compute_A() {
    // A[n] = 2^(2^n - 1)
    int mod_expo = MOD - 1;
    int curr_expo = 1;
    A[0] = Z(1);

    for (int n = 1; n <= N; n++) {
        curr_expo = (2LL * curr_expo) % mod_expo;
        A[n] = Z(2).pow(curr_expo - 1);
    }
}

Z B[N + 1];
void compute_B() {
    // B[n] = A[n] - \sum_{k = 1}^{n - 1} C(n - 1, k - 1) * B[k] * A[n - k]
    assert(A[0] == Z(1));
    for (int n = 1; n <= N; n++) {
        B[n] = A[n];
        for (int k = 1; k < n; k++) {
            B[n] -= comb(n - 1, k - 1) * B[k] * A[n - k];
        }
    }

    // // Minor adjustment
    B[1] -= Z(1);
}

Z C[M + 1][N + 1];

void compute_C() {
    // C_0(x) = 1
    C[0][0] = Z(1);

    // C_1(x) = B(x), where B(x) = \sum_{n = 0}^{N} B[n] * x^n/n!
    for (int n = 0; n <= N; n++) {
        C[1][n] = B[n];
    }

    // C_m(x) = B(x)^m = C_{m - 1}(x) * B(x)
    for (int m = 2; m <= M; m++) {
        for (int n = 0; n <= N; n++) {
            C[m][n] = Z(0);
            for (int k = 0; k <= n; k++) {
                C[m][n] += comb(n, k) * C[m - 1][k] * B[n - k];
            }
        }
    }
}

Z F[M + 1][N + 1];

void compute_F() {
    // F[m][n] = \sum_{k = 1}^{n} comb(n, k) * C[m][k]
    for (int m = 0; m <= M; m++) {
        for (int n = 0; n <= N; n++) {
            F[m][n] = Z(0);
            for (int k = 1; k <= n; k++) {
                F[m][n] += comb(n, k) * C[m][k];
            }
        }
    }
}

Z G(int m, int n) {
    return F[m][n] * inv_fact[m];
}

int main() {
    precompute();
    compute_A();
    compute_B();
    compute_C();
    compute_F();

    cout << G(M, N) << '\n';
}