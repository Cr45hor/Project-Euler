#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

int const N = 10;
int const M = 11;

int64_t dp[1 << (N * 2)][M];
int r_pow[N * 2];

int main() {
    r_pow[0] = 1;
    for (int i = 1; i < N * 2; ++i) {
        r_pow[i] = (r_pow[i - 1] * 10) % M;
    }

    dp[0][0] = 1;

    for (int mask = 0; mask < (1 << (N * 2)); mask++) {
        if (__builtin_popcount(mask) & 1) {
            continue;
        }

        int d = __builtin_popcount(mask) >> 1;
        int pos_limit = N * 2 - (mask == 0);
        for (int i = 0; i < pos_limit; i++) {
            if (!(mask & (1 << i))) {
                for (int j = i + 1; j < pos_limit; j++) {
                    if (!(mask & (1 << j))) {
                        int nmask = mask | (1 << i) | (1 << j);
                        int r_add = d * (r_pow[i] + r_pow[j]) % M;
                        for (int r = 0; r < M; r++) {
                            dp[nmask][(r + r_add) % M] += dp[mask][r];
                        }
                    }
                }
            }
        }
    }

    cout << dp[(1 << (N * 2)) - 1][0] << '\n';
}