#include <iostream>
#include <vector>
#include <cstdint>

const int N = 1e7;

// Store the Lowest Prime Power Factor (lppf) instead of just the smallest prime
int lppf[N + 1];

void sieve() {
    std::vector<int> primes;
    primes.reserve(664580); // Exact number of primes under 1e7 is 664579
    
    for (int i = 2; i <= N; ++i) {
        if (lppf[i] == 0) {
            lppf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (i * p > N) break;
            if (i % p == 0) {
                lppf[i * p] = lppf[i] * p; // Inherit and multiply the prime power
                break;
            } else {
                lppf[i * p] = p; // p is strictly a new lowest prime factor
            }
        }
    }
}

int M(int n) {
    int buf[10];
    int cnt = 0;
    int temp = n;
    
    // 1. O(L) Factorization: No inner while loop needed
    while (temp > 1) {
        int comp = lppf[temp];
        buf[cnt++] = comp;
        temp /= comp;
    }

    if (cnt == 1) return 1;

    // 2. Search Space Halving
    int half_mask = (1 << (cnt - 1)) - 1;
    int prod[256]; 
    prod[0] = 1;
    int max_sol = 1;

    for (int mask = 1; mask <= half_mask; ++mask) {
        int msb = 31 - __builtin_clz(mask);
        int p = prod[mask ^ (1 << msb)] * buf[msb];
        prod[mask] = p;
        
        int q = n / p;
        
        // 3. Iterative 32-bit Modular Inverse
        int u = 1, x = 0;
        int b = q, a = p;
        while (a != 0) {
            int quo = b / a;
            int r = b % a;
            int m = u - quo * x;
            b = a; a = r;
            u = x; x = m;
        }
        if (u < 0) u += p;
        
        // 4. Modulo Elimination & Symmetry Magic
        int solA = q * u; 
        int solB = n + 1 - solA; 
        
        if (solA > max_sol) max_sol = solA;
        if (solB > max_sol) max_sol = solB;
    }
    return max_sol;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();
    
    int64_t res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int n = 2; n <= N; n++) {
        res += M(n);
    }
    
    std::cout << res << '\n';
    return 0;
}