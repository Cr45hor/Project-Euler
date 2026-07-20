#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// long double gives 80-bit precision on Linux x86 (~19 decimal digits)
typedef long double Real;

const int MAXN = 350;
Real inv_pow_fact[MAXN + 1][MAXN + 1];

// Precompute the probability multipliers: 1 / (len^k * k!)
void precompute() {
    for (int len = 1; len <= MAXN; ++len) {
        Real current_pow = 1.0;
        Real current_fact = 1.0;
        inv_pow_fact[len][0] = 1.0;
        
        for (int k = 1; k * len <= MAXN; ++k) {
            current_pow *= len;
            current_fact *= k;
            inv_pow_fact[len][k] = 1.0 / (current_pow * current_fact);
        }
    }
}

// Fast LCM: safely uses 128-bit integers, but delegates GCD to fast 32-bit hardware
unsigned __int128 get_lcm(unsigned __int128 a, int b) {
    unsigned int rem = a % b;
    if (rem == 0) return a;
    
    int x = b, y = rem;
    while (y) {
        int t = x % y;
        x = y;
        y = t;
    }
    return (a / x) * b;
}

Real total_expected = 0;
Real c = 0; // Kahan summation compensation variable

void dfs(int rem, int max_len, unsigned __int128 current_lcm, Real current_prob) {
    // Base Case 1: All elements are partitioned
    if (rem == 0) {
        Real lcm_real = (Real)current_lcm;
        Real term = current_prob * lcm_real * lcm_real;
        
        // Kahan Summation to prevent floating-point drift over 480M additions
        Real y = term - c;
        Real t = total_expected + y;
        c = (t - total_expected) - y;
        total_expected = t;
        return;
    }
    
    // Base Case 2: Only 1-cycles remain. 
    if (max_len == 1) {
        Real next_prob = current_prob * inv_pow_fact[1][rem];
        Real lcm_real = (Real)current_lcm; // LCM doesn't change when adding 1s
        Real term = next_prob * lcm_real * lcm_real;
        
        Real y = term - c;
        Real t = total_expected + y;
        c = (t - total_expected) - y;
        total_expected = t;
        return;
    }

    // Branch 1: We take ZERO cycles of length max_len
    dfs(rem, max_len - 1, current_lcm, current_prob);

    // Branch 2: We take k >= 1 cycles of length max_len
    if (max_len <= rem) {
        // We only calculate the new LCM once for all k > 0
        unsigned __int128 next_lcm = get_lcm(current_lcm, max_len);
        
        for (int k = 1; k * max_len <= rem; ++k) {
            dfs(rem - k * max_len, max_len - 1, next_lcm, current_prob * inv_pow_fact[max_len][k]);
        }
    }
}

void solve(int n) {
    total_expected = 0;
    c = 0; // Reset Kahan compensation
    
    dfs(n, n, 1, 1.0);
    
    cout << "N = " << n << "\n";
    cout << "Expected value : " << scientific << setprecision(9) << total_expected << "\n\n";
}

int main(int argc, char *argv[]) {
    precompute();
    
    int n = stoi(argv[1]);
    solve(n);
    
    return 0;
}