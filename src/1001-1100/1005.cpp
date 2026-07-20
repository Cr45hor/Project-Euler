#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>

using namespace std;

const int N = 2026;
const int MOD = 1e9;

int is_prime[N];
vector<int> primes;

void sieve() {
    for (int i = 3; i < N; i += 2) {
        is_prime[i] = 1;
    }
    is_prime[2] = 1;
    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i * 2) {
                is_prime[j] = 0;
            }
        }
    }

    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}

// dp[s][k] = number of increasing sequences such that sum of elements is s and the first element is the k-th prime number
int64_t dp[N + 1][N];
int64_t tot = 0;
int M = 0;

void compute_dp() {
    M = (int)primes.size();
    primes.insert(primes.begin(), 0);
    dp[0][0] = 1;
    assert(primes[1] == 2);

    for (int s = 2; s <= N; s++) {
        for (int k = 1; k <= M; k++) {
            int p = primes[k];
            if (p > s) {
                break;
            }
            else if (p == s) {
                dp[s][k] = 1;
            }
            else {
                for (int l = k + 1; l <= M; l++) {
                    dp[s][k] += dp[s - p][l];
                }
            }
        }
    }

    for (int k = 1; k <= M; k++) {
        tot += dp[N][k];
    }
}

void gen(int curr_sum, int curr_k, int64_t target, vector<int>& seq, int64_t& res) {
    for (int k = curr_k; k <= M; k++) {
        if (target > dp[curr_sum][k]) {
            target -= dp[curr_sum][k];
        }
        else {
            seq.push_back(primes[k]);
            res = 1LL * res * primes[k] % MOD;
            if (curr_sum == primes[k]) {
                return;
            }
            else {
                gen(curr_sum - primes[k], k + 1, target, seq, res);
                return;
            }
        }
    }
}

int main() {
    sieve();
    compute_dp();
    
    int64_t target = (tot + 1) / 2, res = 1;
    vector<int> seq;
    gen(N, 1, target, seq, res);
    cout << res << '\n';
}