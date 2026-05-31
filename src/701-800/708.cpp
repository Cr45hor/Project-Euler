#include <iostream>
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <cassert>

using namespace std;

int64_t const N = 1e5;

int64_t test(int64_t n) {
    int64_t ans = 0;
    for (int64_t x = 1; x <= n; x++) {
        ans += n / x;
    }
    return ans;
}

int spf[N + 1];

void sieve() {
    for (int i = 1; i <= N; i++) {
        spf[i] = i;
    }

    for (int i = 2; i * i <= N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int factorize(int64_t n, int buffer[64]) {
    int idx = 0;
    while (n > 1) {
        buffer[idx++] = spf[n];
        n /= spf[n];
    }
    return idx;
}

int64_t dp[1 << 20];
int64_t cnt[N + 1];
int64_t ans = 0;

void update(int64_t n) {
    int buffer[64];
    int idx = factorize(n, buffer);

    dp[0] = 1;
    for (int mask = 1; mask < (1 << idx); mask++) {
        int msb = 31 - __builtin_clz(mask);
        dp[mask] = dp[mask ^ (1 << msb)] * buffer[msb];
    }

    for (int mask = 0; mask < (1 << idx); mask++) {
        cnt[dp[mask]]++;
    }
    
    ans += (1 << idx);
}

int main() {
    sieve();

    for (int i = 1; i <= N; i++) {
        update(i);
    }
    assert(ans == accumulate(cnt + 1, cnt + N + 1, int64_t(0)));
    
    cout << cnt[1] << '\n';
    cout << cnt[2] << '\n';
    cout << cnt[3] << '\n';
    cout << cnt[4] << '\n';
    cout << cnt[6] << '\n';
}