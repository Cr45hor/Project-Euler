#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int const N = 1e6;

int spf[N];

void sieve() {
    for (int x = 1; x < N; x++) {
        spf[x] = x;
    }
    for (int x = 4; x < N; x += 2) {
        spf[x] = 2;
    }
    for (int x = 3; x * x < N; x += 2) {
        if (spf[x] == x) {
            for (int y = x * x; y < N; y += x * 2) {
                if (spf[y] == y) {
                    spf[y] = x;
                }
            }
        }
    }
}

int binpow(int a, int n, int mod) {
    int res = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = (int64_t)res * a % mod;
        }
        a = (int64_t)a * a % mod;
    }
    return res;
}

int buf[8];

int factor(int n) {
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        while (n % p == 0) {
            n /= p;
        }
        buf[cnt++] = p;
    }
    return cnt;
}

int mandatory[N];

vector<int> multiples[N];
vector<int> factors[N];

void update(int n) {
    if (spf[n] == n) {
        mandatory[n]++;
        return;
    }

    int l = factor(n);
    if (all_of(buf, buf + l, [&](int p) { return mandatory[p] == 0; })) {
        factors[n] = vector<int>(buf, buf + l);
        for (int p: factors[n]) {
            multiples[p].push_back(n);
        }
    }
}

bool covered[N] = {false};
int active_count[N] = {0};

int main() {
    sieve();

    for (int n = 3; n <= N; n += 10) {
        update(n);
    }

    // 1. Setup Priority Queue for Lazy Deletion
    // Stores pairs of {count, -prime}
    priority_queue<pair<int, int>> pq;

    for (int p = 2; p < N; p++) {
        if (spf[p] == p && multiples[p].size() > 0) {
            active_count[p] = multiples[p].size();
            pq.push({active_count[p], -p});
        } 
    }

    // 2. The Optimized Greedy Loop
    while (!pq.empty()) {
        auto [count, neg_p] = pq.top();
        pq.pop();
        int p = -neg_p;

        // LAZY DELETION: If the count in the queue doesn't match the true 
        // active count, this is an old, stale record. Ignore it!
        if (count != active_count[p]) continue;
        if (count == 0) break; 

        mandatory[p]++;
        active_count[p] = 0; // Mark this prime as fully processed

        for (int n : multiples[p]) {
            // Instead of vector::erase, just check if it's already covered
            if (!covered[n]) {
                covered[n] = true;
                
                // Since 'n' is now covered, all other primes that divide 'n' 
                // just lost a target. Update their counts.
                for (int q : factors[n]) {
                    if (q != p && active_count[q] > 0) {
                        active_count[q]--;
                        pq.push({active_count[q], -q}); // Push updated state
                    }
                }
            }
        }
    }

    // 3. Final Calculation
    double res = 0;
    for (int p = 2; p < N; p++) {
        if (spf[p] == p && mandatory[p] > 0) {
            res += log(p);
        }
    }

    cout << setprecision(6) << fixed << res << '\n';
}