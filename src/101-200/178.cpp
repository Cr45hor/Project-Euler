#include <iostream>
#include <cassert>
#include <cstdint>
#include <numeric>

using namespace std;

int const N = 40;

int64_t dp[N + 1][1 << 10][10];

int main() {
    for (int d = 1; d <= 9; d++) {
        dp[1][1 << d][d] = 1;
    }

    for (int pos = 1; pos < N; pos++) {
        for (int mask = 0; mask < (1 << 10); mask++) {
            int curr_mask = mask;
            while(curr_mask) {
                int d = __builtin_ctz(curr_mask);
                curr_mask ^= 1 << d;
                
                if (d + 1 <= 9) {
                    dp[pos + 1][mask | (1 << (d + 1))][d + 1] += dp[pos][mask][d];
                }
                if (d - 1 >= 0) {
                    dp[pos + 1][mask | (1 << (d - 1))][d - 1] += dp[pos][mask][d];
                }
            }
        }
    }

    int64_t sum = 0;
    for (int len = 10; len <= N; len++) {
        sum += accumulate(&dp[len][(1 << 10) - 1][0], &dp[len][(1 << 10) - 1][10], int64_t(0));
    }
    cout << sum << '\n';
}