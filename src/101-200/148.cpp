#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

using namespace std;

int const N = 1e9;
int const M = 16;
int const BASE = 7;

int convert(int n, int buf[M]) {
    int len = 0;
    while(n > 0) {
        buf[len++] = n % BASE;
        n /= BASE;
    }
    return len;
}

int64_t dp[M][2][2][2];

int64_t calc(int buf[M], int id, int smaller_n, int smaller_k, int divisible) {
    if (id < 0) {
        return smaller_n && !divisible;
    }

    int64_t& res = dp[id][smaller_n][smaller_k][divisible];
    if (res != -1) {
        return res;
    }
    res = 0;

    int ln = 0;
    int rn = (smaller_n ? BASE - 1 : buf[id]);
    for (int dn = ln; dn <= rn; dn++) {
        int next_smaller_n = smaller_n || (dn < buf[id]);

        int lk = 0;
        int rk = (smaller_k ? BASE - 1 : dn);
        for (int dk = lk; dk <= rk; dk++) {
            int next_smaller_k = smaller_k || (dk < dn);
            int next_divisible = divisible || (dk > dn);
            res += calc(buf, id - 1, next_smaller_n, next_smaller_k, next_divisible);
        }
    }
    return res;
}

int64_t solve(int N) {
    fill(&dp[0][0][0][0], &dp[0][0][0][0] + M * 2 * 2 * 2, -1);
    int buf[M]{};
    int l = convert(N, buf);
    return calc(buf, l - 1, false, false, false);
}

int main() {
    assert(solve(100) == 2361);
    cout << solve(N) << '\n';
    return 0;
}