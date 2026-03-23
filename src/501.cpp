/*
Portion of this code is copied from Sgtlaugh's algovault repository
(specifically the Lehmer's prime counting function implementation) 
and is licensed under MIT License.
*/

#include <iostream>
#include <math.h>
#include <vector>
#include <bitset>
#include <assert.h>
#include <stdint.h>

using namespace std;

const int MAXV = 20000010;

const int MAXP = 7;
const int MAXN = 50;
const int MAXM = 2 * 3 * 7 * 5 * 11 * 13 * 17; /// Product of the first MAXP primes

constexpr auto fast_div = [](const long long& a, const int& b) ->long long {return double(a) / b + 1e-9;};

vector<int> primes;
bitset<MAXV> is_prime;
int prod[MAXP], pi[MAXV], dp[MAXN][MAXM];

void sieve(){
    is_prime[2] = true;
    for (int i = 3; i < MAXV; i += 2) is_prime[i] = true;

    for (int i = 3; i * i < MAXV; i += 2){
        for (int j = i * i; is_prime[i] && j < MAXV; j += (i << 1)){
            is_prime[j] = false;
        }
    }

    for (int i = 1; i < MAXV; i++){
        pi[i] = pi[i - 1] + is_prime[i];
        if (is_prime[i]) primes.push_back(i);
    }
}

void gen(){
    int i, j;
    assert(MAXN >= MAXP);

    sieve();
    for (prod[0] = primes[0], i = 1; i < MAXP; i++){
        prod[i] = prod[i - 1] * primes[i];
    }

    for (i = 0; i < MAXM; i++) dp[0][i] = i;
    for (i = 1; i < MAXN; i++){
        for (j = 1; j < MAXM; j++){
            dp[i][j] = dp[i - 1][j] - dp[i - 1][fast_div(j, primes[i - 1])];
        }
    }
}

uint64_t phi(long long m, int n){
    if (!n) return m;
    if (n < MAXN && m < MAXM) return dp[n][m];
    if (n < MAXP) return dp[n][m % prod[n - 1]] + fast_div(m, prod[n - 1]) * dp[n][prod[n - 1]];

    long long p = primes[n - 1];
    if (m < MAXV && p * p >= m) return pi[m] - n + 1;
    if (p * p * p < m || m >= MAXV) return phi(m, n - 1) - phi(fast_div(m, p), n - 1);

    int lim = pi[(int)sqrt(0.5 + m)];
    uint64_t res = pi[m] - (lim + n - 2) * (lim - n + 1) / 2;
    for (int i = n; i < lim; i++){
        res += pi[fast_div(m, primes[i])];
    }

    return res;
}

uint64_t lehmer(long long n){
    if (n < MAXV) return pi[n];

    int s = sqrt(0.5 + n), c = cbrt(0.5 + n);
    uint64_t res = phi(n, pi[c]) + pi[c] - 1;
    for (int i = pi[c]; i < pi[s]; i++){
        res -= lehmer(fast_div(n, primes[i])) - i;
    }

    return res;
}

uint64_t N = 1e12;

int main() {
    gen();

    uint64_t res = 0;
    // Case 1: x = p^7 <= N
    int limit1 = (int)pow(N, 1.0 / 7);
    res += lehmer(limit1);

    // Case 2: x = p^3 * q <= N, p < q
    // Case 3: x = p * q^3 <= N, p < q
    int limit23 = (int)pow(N, 1.0 / 4);
    for (int p = 2; p <= limit23; p++) {
        if (is_prime[p]) {
            uint64_t q2_limit = (uint64_t)N / (1LL * p * p * p);
            if (q2_limit > (uint64_t)p) {
                res += lehmer(q2_limit) - lehmer(p);
            }
            uint64_t q3_limit = (uint64_t)pow(N / p, 1.0 / 3);
            if (q3_limit > (uint64_t)p) {
                res += lehmer(q3_limit) - lehmer(p);
            }
        }
    }

    // Case 4: x = p * q * r <= N
    int limit4 = (int)pow(N, 1.0 / 3);
    for (int p = 2; p <= limit4; p++) {
        if (is_prime[p]) {
            uint64_t cap = sqrt(N / p);
            for (int q = p + 1; (uint64_t)q <= cap; q++) {
                if (is_prime[q]) {
                    uint64_t r_limit = (uint64_t)N / (1LL * p * q);
                    if (r_limit > (uint64_t)q) {
                        res += lehmer(r_limit) - lehmer(q);
                    }
                }
            }
        }
    }

    cout << res << endl;

    return 0;
}