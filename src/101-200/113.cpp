#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <string>

using namespace std;

int const N = 100;

int64_t dp[N + 1][10];

bool check(int64_t n, int ascending = 1) {
    if (n < 10) {
        return true;
    }
    int prev_prev_d = (ascending ? 9 : 0);
    int prev_d = n % 10;
    int factor = (ascending ? 1 : -1);
    n /= 10;
    while (n > 0) {
        int d = n % 10;
        // cout << d << ' ' << prev_d << ' ' << prev_prev_d << '\n';
        if (!(d * factor <= prev_d * factor && prev_d * factor <= prev_prev_d * factor)) {
            return false;
        }
        prev_prev_d = prev_d;
        prev_d = d;
        n /= 10;
    }
    return true;
}

void precompute() {
    dp[0][0] = 1;
    for (int i = 1; i <= N; i++) {
        for (int d = 0; d < 10; d++) {
            for (int prev_d = 0; prev_d <= d; prev_d++) {
                dp[i][d] += dp[i - 1][prev_d];
            }
        }
    }
}

int64_t f(int n) {
    int64_t ans = 0;
    for (int d = 0; d < 10; d++) {
        ans += dp[n][d];
    }
    return ans;
}

int64_t brute_force(int n) {
    int64_t ans[2][2] = {};
    for (int64_t x = 1; ; x++) {
        if (to_string(x).size() > (size_t)n) {
            break;
        }
        ans[check(x, 1)][check(x, 0)]++;
    }
    cout << "ans[0][0] = " << ans[0][0] << '\n';
    cout << "ans[1][0] = " << ans[1][0] << '\n';
    cout << "ans[0][1] = " << ans[0][1] << '\n';
    cout << "ans[1][1] = " << ans[1][1] << '\n';
    return ans[1][0] + ans[0][1] + ans[1][1];
}

int calc1(int d1, int n) {
    int res = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        res += (d1 + __builtin_popcount(mask) <= 9);
    }
    return res;
}

int main() {
    precompute();

    // cout << f(6) << '\n';
    cout << brute_force(6) << '\n';
    cout << f(6) << '\n';
    return 0;
}