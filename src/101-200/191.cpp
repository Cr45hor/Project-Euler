#include <iostream>
#include <vector>
#include <cstdint>
#include <numeric>

using namespace std;

int const N = 30;

int64_t dp[N + 1][3][2];

int main() {
    dp[0][0][0] = 1;

    for (int t = 0; t < N; t++) {
        for (int c = 0; c < 3; c++) {
            for (int l = 0; l < 2; l++) {
                if (dp[t][c][l] == 0) {
                    continue;
                }

                int64_t& curr_state = dp[t][c][l];

                // Case 1: O
                dp[t + 1][0][l] += curr_state;

                // Case 2: A
                if (c + 1 < 3) {
                    dp[t + 1][c + 1][l] += curr_state;
                }

                // Case 3: L
                if (l + 1 < 2) {
                    dp[t + 1][0][l + 1] += curr_state;
                }
            }
        }
    }

    cout << accumulate(&dp[N][0][0], &dp[N][0][0] + 3 * 2, int64_t(0)) << '\n';

    return 0;
}