#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

const int N = 1e6;
const int POW_FACTOR = 1234567890;

int spf[N + 1];
int freq[N + 1];

int64_t final_N = 0;
const int64_t MOD = 1e18;

void sieve() {
    for (int i = 1; i <= N; i += 2) {
        spf[i] = i;
    }
    for (int i = 2; i <= N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i <= N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += 2 * i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int factor(int n, int buffer[16]) {
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        int k = 0;
        while (spf[n] == p) {
            n /= p;
            k++;
        }
        buffer[cnt << 1] = p;
        buffer[cnt << 1 | 1] = k;
        cnt++;
    }
    return cnt;
}

inline int64_t v(int64_t p, int64_t n) {
    int64_t res = 0;
    while(n > 0) {
        n /= p;
        res += n;
    }
    return res;
}

inline void update(int p, int k) {
    freq[p] += k;

    int64_t target = 1LL * freq[p] * POW_FACTOR;
    
    // Fast path: if the current global max already satisfies the
    // requirement for this prime, we can skip the binary search entirely.
    if (v(p, final_N) >= target) {
        return; 
    }

    int64_t L = target * (p - 1) + 1;
    if (L <= final_N) {
        L = final_N + 1;
    }
    int64_t R = target * (p - 1) + 64LL * (p - 1);

    while(L < R) {
        int64_t mid = L + (R - L) / 2;
        if (v(p, mid) >= target) {
            R = mid;
        } else {
            L = mid + 1;
        }
    }
    
    final_N = L;
    assert(final_N < MOD);
}

inline void update(int n) {
    int buffer[16];
    int cnt = factor(n, buffer);
    for (int i = 0; i < cnt; i++) {
        int p = buffer[i << 1];
        int k = buffer[i << 1 | 1];
        update(p, k);
    }
}

int main() {
    sieve();

    int64_t res = 0;
    for (int n = 2; n <= N; n++) {
        update(n);
        if (n >= 10) {
            res += final_N;
            if (res >= MOD) { 
                res -= MOD;
            }
        }
    }
    
    cout << res << '\n';
    return 0;
}