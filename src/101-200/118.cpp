#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cstdint>
#include <gmpxx.h>
#include <vector>

using namespace std;

int const N = 10;

int cnt_prime[1 << N];
int total = 0;

void precompute() {
    vector<int> mark(N);

    for (int k = 1; k <= N; k++) {
        fill(mark.begin(), mark.end(), 0);
        fill(mark.end() - k, mark.end(), 1);
        do {
            vector<int> digits;
            for (int i = 0; i < N; i++) {
                if (mark[i]) digits.push_back(i);
            }
            // Reject digit 0
            if (!mark[0]) {
                do {
                    int n = 0;
                    int mask = 0;
                    for (int d : digits) {
                        n = n * 10 + d;
                        mask |= 1 << d;
                    }
                    if (mpz_probab_prime_p(mpz_class(n).get_mpz_t(), 25) > 0) {
                        cnt_prime[mask]++;
                        total++;
                        // cout << n << ' ' << mask << '\n';
                    }
                } while(next_permutation(digits.begin(), digits.end()));
            }
        } while(next_permutation(mark.begin(), mark.end()));
    }
}

int64_t dp[1 << N];

int main() {
    precompute();
    cout << "TOTAL: " << total << '\n';

    dp[1] = 1; // Suppose 0 are already in the set
    for (int mask = 1; mask + 1 < (1 << N); mask++) {
        // Find smallest bit in current mask that is not set
        int d = __builtin_ctz(~mask);

        // Iterate all submasks of complement of mask that contain d
        for (int submask = (1 << d); submask < (1 << N); submask = (submask + 1) | (1 << d)) {
            if ((submask & mask) == 0) {
                dp[mask | submask] += dp[mask] * cnt_prime[submask];
            }
        }
    }

    cout << dp[(1 << N) - 1] << '\n';
}