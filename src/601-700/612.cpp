#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../Z.hpp"
int const MOD = 1000267129;

using namespace std;

int const N = 18;
int const M = 10;

// dp[n][mask] = number of sequences of length n with the given mask of digits used without leading zeros
Z dp[N + 1][1 << M];
Z freq[1 << M];
Z sum[1 << M];

int main() { 
    // Base case: n = 1 (must not use 0 here)
    for (int d = 1; d < M; d++) {
        dp[1][1 << d] = 1;
    }

    // For n >= 2, since the first digit is not zero, we can append any digit (including zero) to the previous sequences
    for (int n = 2; n <= N; n++) {
        for (int mask = 0; mask < (1 << M); mask++) {
            for (int d = 0; d < M; d++) {
                dp[n][mask | (1 << d)] += dp[n - 1][mask];
            }
        }
    }

    for (int n = 1; n <= N; n++) {
        for (int mask = 0; mask < (1 << M); mask++) {
            freq[mask] += dp[n][mask];
        }
    }

    Z tot = accumulate(freq, freq + (1 << M), Z(0));
    assert(tot == Z(10).pow(N) - 1);

    // DP SOS
    for (int mask = 0; mask < (1 << M); mask++) {
        sum[mask] = freq[mask];
    }
    for (int i = 0; i < M; i++) {
        for (int mask = 0; mask < (1 << M); mask++) {
            if (mask & (1 << i)) {
                sum[mask] += sum[mask ^ (1 << i)];
            }
        }
    }

    Z res = tot * (tot - 1);
    for (int mask = 0; mask < (1 << M); mask++) {
        res -= freq[mask] * sum[(1 << M) - 1 - mask];
    }
    cout << res * Z((MOD + 1) / 2) << "\n";
}