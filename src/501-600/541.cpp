#include <iostream>
#include <vector>
#include <gmpxx.h>

using namespace std;

const int MAX_DEPTH = 8; // Depth 8 is plenty for p=137
long p = 137; // Changed from long long to long to fix GMP ambiguity

mpz_class max_J = 0;
mpz_class max_M = 0;
vector<mpz_class> MOD(MAX_DEPTH + 1);

// Extended Euclidean Algorithm for GMP
mpz_class modInverse(mpz_class a, mpz_class m) {
    mpz_class res;
    mpz_invert(res.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t());
    return res;
}

// Compute sum_{i=1}^{p-1} (1 / i^m) mod mod_val
mpz_class sum_inverse_powers(int m, mpz_class mod_val) {
    mpz_class total = 0;
    for (int i = 1; i < p; ++i) {
        mpz_class inv = modInverse(i, mod_val);
        mpz_class term = 1;
        for (int k = 0; k < m; ++k) {
            term = (term * inv) % mod_val;
        }
        total = (total + term) % mod_val;
    }
    return total;
}

// Compute sum_{j=0}^{q-1} j^m mod mod_val using a simple DP/Pascal approach
mpz_class power_sum(mpz_class q, int m, mpz_class mod_val) {
    if (m == 0) return q % mod_val;
    if (m == 1) {
        mpz_class res = (q * (q - 1)) / 2;
        return res % mod_val;
    }
    if (m == 2) {
        mpz_class res = (q * (q - 1) * (2 * q - 1)) / 6;
        return res % mod_val;
    }
    if (m == 3) {
        mpz_class res = (q * (q - 1)) / 2;
        res = (res * res);
        return res % mod_val;
    }
    
    // Fallback naive loop for higher m if q is small
    mpz_class total = 0;
    for (mpz_class j = 0; j < q; ++j) {
        mpz_class term = 1;
        for (int k = 0; k < m; ++k) {
            term = (term * j) % mod_val;
        }
        total = (total + term) % mod_val;
    }
    return total;
}

// Compute the exact p-adic carry S(q) using the Taylor expansion
mpz_class compute_carry(mpz_class q, int depth, mpz_class mod_val) {
    mpz_class S_q = 0;
    
    // We only need terms up to the current remaining depth
    for (int m = 0; m < MAX_DEPTH - depth; ++m) {
        mpz_class p_pow = 1;
        for (int i = 0; i < m; ++i) p_pow *= p;
        
        mpz_class inv_sum = sum_inverse_powers(m + 1, mod_val);
        mpz_class pow_sum = power_sum(q, m, mod_val);
        
        mpz_class term = (p_pow * inv_sum) % mod_val;
        term = (term * pow_sum) % mod_val;
        
        if (m % 2 != 0) { // (-1)^m
            term = (mod_val - term) % mod_val;
        }
        
        S_q = (S_q + term) % mod_val;
    }
    return S_q;
}

void dfs(mpz_class q, mpz_class V_q, int level) {
    if (level >= MAX_DEPTH) return;
    
    mpz_class current_mod = MOD[MAX_DEPTH - level];
    
    // Compute the EXACT base value including the Taylor series carry
    mpz_class V_next_base = V_q / p; 
    mpz_class carry = compute_carry(q, level, current_mod);
    V_next_base = (V_next_base + carry) % current_mod;
    
    mpz_class sum_r = 0;
    
    // Changed r to long to fix GMP ambiguity
    for (long r = 0; r < p; ++r) {
        if (r > 0) {
            mpz_class target = p * q + r;
            sum_r = (sum_r + modInverse(target, current_mod)) % current_mod;
        }
        
        mpz_class V_pq_r = (V_next_base + sum_r) % current_mod;
        mpz_class n = p * q + r;
        
        if (n > 0) {
            if (n > max_M) max_M = n;
            
            if (V_pq_r % p == 0) {
                if (n > max_J) max_J = n;
                dfs(n, V_pq_r, level + 1);
            }
        }
    }
}

int main() {
    MOD[0] = 1;
    for (int i = 1; i <= MAX_DEPTH; ++i) {
        MOD[i] = MOD[i - 1] * p;
    }

    cout << "Executing GMP-backed Taylor DFS for p = " << p << "...\n\n";
    
    dfs(0, 0, 0);
    
    cout << "Max n in J_p (Numerator divisible by p): " << max_J.get_str() << "\n";
    cout << "Max n for PE 541 (Denominator NOT divisible by p): " << max_M.get_str() << "\n";

    return 0;
}