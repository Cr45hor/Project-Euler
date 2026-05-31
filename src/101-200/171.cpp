#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdint>

#include "Z.hpp"

using namespace std;

int MOD = 1e9;

int const N = 20;
int const MAX_S = N * (9 * 9);

pair<Z, Z> dp[N][MAX_S + 1];
int vis[N][MAX_S + 1];

bool is_square(int x) {
    int r = sqrt(x);
    return r * r == x;
}

pair<Z, Z> calc(int n, int s) {
    if (n >= N) {
        return {Z(is_square(s)), Z(0)};
    }

    if (vis[n][s]) {
        return dp[n][s];
    }

    auto& [num, sum] = dp[n][s];
    for (int d = 0; d <= 9; d++) {
        int ns = s + d * d;
        auto [nnum, nsum] = calc(n + 1, ns);
        num += nnum;
        sum += nsum + nnum * Z(d) * Z(10).pow(N - n - 1);
    }
    vis[n][s] = 1;
    return dp[n][s];
}

int main() {
    auto [num, sum] = calc(0, 0);
    cout << sum << '\n';

    return 0;
}

