#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;

// Native 128-bit integers handle values up to ~3.4 * 10^38
typedef unsigned __int128 u128;

const int64_t N = 99999999019; // Target limit
const int K = 20000000;        // Sieve limit ~ N^(2/3)

vector<int> primes;
vector<bool> is_prime;
vector<int> phi_val;
vector<u128> F_pre;

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

    // Compute prefix sums F(x) = sum(i * phi(i)) up to K
    for (int i = 1; i <= K; ++i) {
        F_pre[i] = F_pre[i - 1] + (u128)i * phi_val[i];
    }
}

unordered_map<int64_t, u128> memo;

u128 get_F(int64_t x) {
    if (x <= K) return F_pre[x];
    if (memo.count(x)) return memo[x];

    u128 x_u = x;
    
    // Evaluate x(x+1)(2x+1) / 6 safely
    u128 res = x_u * (x_u + 1) / 2;
    res = res * (2 * x_u + 1) / 3;

    u128 sub = 0;
    for (int64_t l = 2, r; l <= x; l = r + 1) {
        int64_t v = x / l;
        r = x / v;
        
        // Sum of d in [l, r]
        u128 count = r - l + 1;
        u128 sum_d = count * (l + r) / 2;
        
        sub += sum_d * get_F(v);
    }

    return memo[x] = res - sub;
}

u128 get_S(int64_t n) {
    u128 sum_val = 0;
    for (int64_t l = 1, r; l <= n; l = r + 1) {
        int64_t v = n / l;
        r = n / v;
        
        u128 f_r = get_F(r);
        u128 f_l = get_F(l - 1);
        
        sum_val += (u128)v * (f_r - f_l);
    }
    return (n + sum_val) / 2;
}

// Convert to string to guarantee exact digit output
string to_string(u128 n) {
    if (n == 0) return "0";
    string s;
    while (n > 0) {
        s += (char)('0' + (n % 10));
        n /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    // Run the linear sieve to precompute up to K
    sieve();
    
    // Validate with the small test case
    assert(to_string(get_S(100)) == "122726");
    
    // Compute for massive N
    cout << "Computing S(" << N << ")...\n";
    u128 result = get_S(N); // 121793824651878334823435485978083
    cout << "S(" << N << ") % 999999017 = " << to_string(result % 999999017) << "\n";

    return 0;
}