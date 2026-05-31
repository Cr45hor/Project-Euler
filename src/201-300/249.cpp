#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

const int MAX_PRIME = 4999;
const int SUM_PRIME = 1548136;
const int64_t MOD = 10000000000000000LL;

bool is_prime[SUM_PRIME + 1];
vector<int> primes;

void sieve() {
    fill(is_prime, is_prime + SUM_PRIME + 1, true);
    
    is_prime[0] = is_prime[1] = false;
    for (int i = 4; i <= SUM_PRIME; i += 2) {
        is_prime[i] = false;
    }
    for (int i = 3; i * i <= SUM_PRIME; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j <= SUM_PRIME; j += 2 * i) {
                is_prime[j] = false;
            }
        }
    }

    for (int p = 2; p <= MAX_PRIME; p++) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}

int64_t dp[SUM_PRIME + 1];

int main() {
    sieve();

    dp[0] = 1;
    for (int p: primes) {
        for (int s = SUM_PRIME; s >= p; s--) {
            dp[s] = (dp[s] + dp[s - p]) % MOD;
        }
    }
    
    int64_t res = 0;
    for (int s = 0; s <= SUM_PRIME; s++) {
        if (is_prime[s]) {
            res = (res + dp[s]) % MOD;
        }
    }

    cout << res << '\n';
    return 0;
}