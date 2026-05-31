#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

const long long N = 110000000;

// Extended Euclidean Algorithm to find modular inverse
long long modInverse(long long a, long long m) {
    long long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long long q = a / m;
        long long t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

int main() {
    // // Fast I/O
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    long long max_k = (N - 2) / 3;
    long long K = -1;
    
    // 1. Calculate the strict mathematical cutoff for K
    for (long long k = 0; k <= max_k; k++) {
        long long M = N - (3 * k + 2);
        if (M < 0) break;
        __int128 X = (__int128)(8 * k + 5) * (k + 1) * (k + 1);
        __int128 RHS = (__int128)4 * M * M * M;
        // Derived from AM-GM: Minimum possible (b+c) is achieved when 27 * X = 4 * M^3
        if (27 * X > RHS) {
            break;
        }
        K = k;
    }
    cout << "Cut off K: " << K << '\n';
    assert(K > 0);

    // 2. Precompute the Smallest Prime Factor (SPF) mapping for fast factorization
    int max_val = max((long long)K + 1, (long long)sqrt(8.0 * K + 5.0) + 1);
    vector<int> spf(max_val + 1);
    for (int i = 2; i <= max_val; i++) spf[i] = i;
    for (int i = 2; i * i <= max_val; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= max_val; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    // 3. Sieve to find roots of highest square factors for 8k + 5
    vector<int> S(K + 1, 1);
    for (int p = 3; p * p <= 8 * K + 5; p += 2) {
        if (spf[p] == p) { 
            long long pp = (long long)p * p;
            long long current_pp = pp;
            while (current_pp <= 8LL * K + 5) {
                long long inv8 = modInverse(8, current_pp);
                long long k0 = ((-5LL % current_pp) + current_pp) % current_pp;
                k0 = (k0 * inv8) % current_pp;
                
                for (long long k = k0; k <= K; k += current_pp) {
                    S[k] *= p;
                }
                
                if (current_pp > (8LL * K + 5) / pp) break;
                current_pp *= pp;
            }
        }
    }

    int64_t res = 0;
    
    // 4. Fast factorization & triplet check (Parallelized)
    // Create a parallel region. 'res' is safely accumulated across threads using reduction(+:res)
    #pragma omp parallel reduction(+:res)
    {
        // These vectors are created ONCE PER THREAD, avoiding inner-loop allocations
        // while remaining completely thread-safe.
        vector<pair<long long, int>> local_factors;
        local_factors.reserve(64);
        vector<long long> local_divisors;
        local_divisors.reserve(512);

        // Distribute the loop iterations among the threads. 
        // We use a dynamic schedule because numbers with many prime factors take longer to process.
        #pragma omp for schedule(dynamic, 1024)
        for (long long k = 0; k <= K; k++) {
            long long max_B = 1LL * (k + 1) * S[k];
            long long min_C = (8LL * k + 5) / (1LL * S[k] * S[k]);
            long long max_sum = N - (3LL * k + 2);

            local_factors.clear();
            int temp = k + 1;
            while (temp > 1) {
                int p = spf[temp];
                int count = 0;
                while (temp % p == 0) { count++; temp /= p; }
                local_factors.push_back({p, count});
            }

            temp = S[k];
            while (temp > 1) {
                int p = spf[temp];
                int count = 0;
                while (temp % p == 0) { count++; temp /= p; }
                local_factors.push_back({p, count});
            }

            // Clean overlapping primes by merging
            if (local_factors.size() > 1) {
                sort(local_factors.begin(), local_factors.end());
                int write_idx = 0;
                for (size_t i = 1; i < local_factors.size(); i++) {
                    if (local_factors[i].first == local_factors[write_idx].first) {
                        local_factors[write_idx].second += local_factors[i].second;
                    } else {
                        write_idx++;
                        local_factors[write_idx] = local_factors[i];
                    }
                }
                local_factors.resize(write_idx + 1);
            }

            local_divisors.clear();
            local_divisors.push_back(1);
            for (auto& f : local_factors) {
                int sz = local_divisors.size();
                long long p_pow = 1;
                for (int i = 1; i <= f.second; i++) {
                    p_pow *= f.first;
                    for (int j = 0; j < sz; j++) {
                        local_divisors.push_back(local_divisors[j] * p_pow);
                    }
                }
            }

            for (long long b : local_divisors) {
                if (b > max_sum) continue;
                long long max_B_b = max_B / b;
                
                __int128 c = (__int128)min_C * max_B_b * max_B_b;
                if (c <= max_sum - b) {
                    res++; // Safe to increment directly because of reduction(+:res)
                }
            }
        }
    }

    cout << res << '\n';
    return 0;
}