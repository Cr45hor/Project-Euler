#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned long long ull;

// Fast modular exponentiation
ull modpow(ull base, ull exp, ull mod) {
    ull res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    ull N = 10000000;
    
    // Arrays to maintain the running divided value and the maximum prime found
    vector<ull> val(N + 1);
    vector<ull> largest_prime(N + 1, 1);
    
    for (ull k = 1; k <= N; ++k) {
        val[k] = 4 * k * k + 1;
    }
    
    // The maximum possible prime factor we need to sieve is sqrt(4 * 10^14 + 1)
    ull MAX_PRIME = 20000000; 
    vector<bool> is_prime(MAX_PRIME + 1, true);
    is_prime[0] = is_prime[1] = false;
    
    // Standard prime sieve up to 2 * 10^7
    for (ull p = 2; p * p <= MAX_PRIME; ++p) {
        if (is_prime[p]) {
            for (ull i = p * p; i <= MAX_PRIME; i += p) {
                is_prime[i] = false;
            }
        }
    }
    
    for (ull p = 2; p <= MAX_PRIME; ++p) {
        if (!is_prime[p]) continue;
        
        // We only care about primes p = 1 (mod 4)
        if (p % 4 == 1) {
            
            // 1. Find a quadratic non-residue 'a'
            ull a = 2;
            while (modpow(a, (p - 1) / 2, p) != p - 1) {
                a++;
            }
            
            // 2. Compute the square root of -1 mod p
            ull S = modpow(a, (p - 1) / 4, p);
            
            // 3. Compute roots for k: k = +/- S * 2^-1 mod p
            ull inv2 = (p + 1) / 2;
            ull r1 = (S * inv2) % p;
            ull r2 = p - r1;
            
            // 4. Sieve the array for both roots
            ull roots[2] = {r1, r2};
            for (ull r : roots) {
                ull start_k = (r == 0) ? p : r;
                for (ull k = start_k; k <= N; k += p) {
                    while (val[k] % p == 0) {
                        val[k] /= p; // Remove the factor completely
                    }
                    if (p > largest_prime[k]) {
                        largest_prime[k] = p;
                    }
                }
            }
        }
    }
    
    ull total_sum = 0;
    for (ull k = 1; k <= N; ++k) {
        // If the remaining value is > 1, it must be a single prime factor > 2 * 10^7
        if (val[k] > 1) {
            if (val[k] > largest_prime[k]) {
                largest_prime[k] = val[k];
            }
        }
        if (k <= 100) {
            cout << "k = " << k << ", largest prime factor = " << largest_prime[k] << "\n";
        }
        total_sum += largest_prime[k];
    }
    
    cout << "S(10^7) = " << total_sum << "\n";
    return 0;
}