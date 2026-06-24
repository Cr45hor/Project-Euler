#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cassert>

using namespace std;

const int64_t N = 99999999019;
const int64_t MOD = 999999017;
const int K = 20000000;

// Modular Inverses for division
const int64_t INV2 = 499999509; // (MOD + 1) / 2
const int64_t INV6 = 166666503; // (MOD + 1) / 6

vector<int> primes;
vector<bool> is_prime;
vector<int> phi_val;
vector<int64_t> F_pre;

void sieve() {
    is_prime.assign(K + 1, true);
    phi_val.assign(K + 1, 0);
    F_pre.assign(K + 1, 0);

    is_prime[0] = is_prime[1] = false;
    phi_val[1] = 1;
    F_pre[1] = 1;

    for (int i = 2; i <= K; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            phi_val[i] = i - 1;
        }
        for (int p : primes) {
            if (i * p > K) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                phi_val[i * p] = phi_val[i] * p;
                break;
            } else {
                phi_val[i * p] = phi_val[i] * (p - 1);
            }
        }
    }

    // Compute prefix sums modulo MOD
    for (int i = 1; i <= K; ++i) {
        F_pre[i] = (F_pre[i - 1] + (int64_t)i * phi_val[i]) % MOD;
    }
}

unordered_map<int64_t, int64_t> memo;

int64_t get_F(int64_t x) {
    if (x <= K) return F_pre[x];
    if (memo.count(x)) return memo[x];

    int64_t x_mod = x % MOD;
    
    // Evaluate x(x+1)(2x+1) / 6 mod MOD
    int64_t res = x_mod * (x_mod + 1) % MOD;
    res = res * (2 * x_mod + 1) % MOD;
    res = res * INV6 % MOD;

    int64_t sub = 0;
    for (int64_t l = 2, r; l <= x; l = r + 1) {
        int64_t v = x / l;
        r = x / v;
        
        // Sum of d in [l, r] mod MOD
        int64_t count = (r - l + 1) % MOD;
        int64_t sum_d = count * ((l + r) % MOD) % MOD * INV2 % MOD;
        
        sub = (sub + sum_d * get_F(v)) % MOD;
    }

    // Ensure positive modulo result
    int64_t ans = (res - sub) % MOD;
    if (ans < 0) ans += MOD;
    
    return memo[x] = ans;
}

int64_t get_S(int64_t n) {
    int64_t sum_val = 0;
    for (int64_t l = 1, r; l <= n; l = r + 1) {
        int64_t v = n / l;
        r = n / v;
        
        int64_t f_r = get_F(r);
        int64_t f_l = get_F(l - 1);
        
        int64_t diff = (f_r - f_l) % MOD;
        if (diff < 0) diff += MOD;
        
        sum_val = (sum_val + (v % MOD) * diff) % MOD;
    }
    
    int64_t ans = ((n % MOD) + sum_val) % MOD * INV2 % MOD;
    return ans;
}

int main() {
    // Run the linear sieve
    sieve();
    
    // Validate with the small test case
    // 122726 % 999999017 is still 122726
    assert(get_S(100) == 122726);
    
    // Compute for massive N
    cout << "Computing S(" << N << ") mod " << MOD << "...\n";
    int64_t result = get_S(N);
    cout << "S(" << N << ") = " << result << "\n";

    return 0;
}