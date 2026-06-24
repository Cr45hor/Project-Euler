#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

int const N = 60000000;
int64_t const MOD = 1e18;

// spf is the only global remaining (240 MB, perfectly safe)
int spf[N + 2];

void sieve() {
    int M = N + 1;
    spf[1] = 1;
    for (int i = 2; i <= M; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i <= M; i += 2) {
        spf[i] = i;
    }
    for (int i = 3; i * i <= M; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= M; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int factorize(int x, int buf[20]) {
    int n = 0;
    while (x > 1) {
        int p = spf[x], k = 0;
        while(x % p == 0) {
            x /= p;
            k++;
        }
        buf[n << 1] = p;
        buf[n << 1 | 1] = k;
        n++;
    }
    assert(n < 10);
    return n;
}

int merge_factorize(int n, int buf1[20], int m, int buf2[20], int buf[40]) {
    int mn = 0;
    int pn = 0, pm = 0;
    while(pn < n || pm < m) {
        if (pn == n) {
            buf[mn << 1] = buf2[pm << 1];
            buf[mn << 1 | 1] = buf2[pm << 1 | 1];
            pm++;
        }
        else if (pm == m) {
            buf[mn << 1] = buf1[pn << 1];
            buf[mn << 1 | 1] = buf1[pn << 1 | 1];
            pn++;
        }
        else if (buf1[pn << 1] < buf2[pm << 1]) {
            buf[mn << 1] = buf1[pn << 1];
            buf[mn << 1 | 1] = buf1[pn << 1 | 1];
            pn++;
        }
        else if (buf1[pn << 1] > buf2[pm << 1]) {
            buf[mn << 1] = buf2[pm << 1];
            buf[mn << 1 | 1] = buf2[pm << 1 | 1];
            pm++;
        }
        else {
            assert(buf1[pn << 1] == buf2[pm << 1]);
            buf[mn << 1] = buf1[pn << 1];
            buf[mn << 1 | 1] = buf1[pn << 1 | 1] + buf2[pm << 1 | 1];
            pn++;
            pm++;
        }
        mn++;
    }
    return mn;
}

int64_t dT(int n) {
    int factor[2] = {n, n + 1};
    int r = n % 2;
    factor[r] >>= 1;

    int buf1[20], buf2[20], buf[40];
    int n1 = factorize(factor[0], buf1);
    int n2 = factorize(factor[1], buf2);
    int m = merge_factorize(n1, buf1, n2, buf2, buf);

    int64_t res = 1;
    for (int i = 0; i < m; i++) {
        res *= (buf[i << 1 | 1] + 1);
    }
    return res;
}

// Fixed: Was previously returning 'int' and truncating 64-bit values!
int64_t normalize(int64_t x) {
    if (x >= MOD) {
        x -= MOD;
    }
    return x;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    sieve();

    vector<int> compressed_lT(N + 1);
    int U = 0; // Will hold the number of unique elements
    
    // Create a local scope to force memory deallocation of lT and S 
    // as soon as we finish building compressed_lT.
    {
        vector<int64_t> lT(N + 1);
        for (int i = 1; i <= N; i++) {
            lT[i] = dT(i);
        }

        vector<int64_t> S(lT.begin() + 1, lT.end());
        sort(S.begin(), S.end());
        S.erase(unique(S.begin(), S.end()), S.end());
        U = S.size(); 

        for (int i = 1; i <= N; i++) {
            compressed_lT[i] = lower_bound(S.begin(), S.end(), lT[i]) - S.begin() + 1;
            assert(compressed_lT[i] > 0 && compressed_lT[i] <= U);
        }
    } // lT and S are destroyed here, freeing ~960 MB of memory!

    // Original logic reversed the array
    reverse(compressed_lT.begin() + 1, compressed_lT.end());

    // Dynamically sized Fenwick tree! Sized to U + 2 instead of N + 1.
    // 2 layers * (U+2) * 8 bytes = Tiny fraction of a Megabyte.
    vector<vector<int64_t>> fw(2, vector<int64_t>(U + 2, 0));

    auto update = [&](int layer, int pos, int64_t val) {
        for (; pos <= U; pos += pos & -pos) {
            fw[layer][pos] = normalize(fw[layer][pos] + val);
        }
    };

    auto query = [&](int layer, int pos) {
        int64_t res = 0;
        for (; pos > 0; pos -= pos & -pos) {
            res = normalize(res + fw[layer][pos]);
        }
        return res;
    };

    int64_t res = 0;
    
    // Combined DP loops! Processing in a single pass entirely eliminates 
    // the need for the massive 960 MB dp[2][N+1] global array.
    for (int i = 1; i <= N; i++) {
        int c = compressed_lT[i];
        
        // Query both layers
        int64_t dp0 = query(0, c - 1);
        int64_t dp1 = query(1, c - 1);
        
        // Update both layers
        update(0, c, 1);
        update(1, c, dp0);
        
        // Accumulate final result from the equivalent of dp[1][i]
        res = normalize(res + dp1);
    }

    cout << res << '\n';
    return 0;
}