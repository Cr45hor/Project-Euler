#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <numeric>
#include <algorithm>

using namespace std;

// We only need primes up to sqrt(10^10) = 100,000
const int MAX_PRIME = 100000; 
bool is_p[MAX_PRIME + 1];
vector<int> primes;

void precompute_primes() {
    for (int i = 1; i <= MAX_PRIME; i += 2) {
        is_p[i] = true;
    }
    is_p[0] = is_p[1] = false;
    is_p[2] = true;

    for (int p = 3; p * p <= MAX_PRIME; p += 2) {
        if (is_p[p]) {
            for (int i = p * p; i <= MAX_PRIME; i += p * 2) {
                is_p[i] = false;
            }
        }
    }
    for (int p = 2; p <= MAX_PRIME; p++) {
        if (is_p[p]) primes.push_back(p);
    }
    cout << "Done precomputing primes up to " << MAX_PRIME << '\n';
}

int64_t M[10];
int64_t N[10];
int64_t S[10];

int main() {
    // Optimize standard I/O operations for a slight speed boost
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    precompute_primes();

    int64_t L_total = 1e9;
    int64_t R_total = 1e10 - 1;
    
    const int64_t CHUNK_SIZE = 100000; 

    for (int64_t L = L_total; L <= R_total; L += CHUNK_SIZE) {
        int64_t R = min(L + CHUNK_SIZE - 1, R_total);
        vector<char> isPrime(R - L + 1, true);

        for (int p : primes) {
            if (1LL * p * p > R) break;
            int64_t start = max(1LL * p * p, (L + p - 1) / p * p);
            
            for (int64_t j = start; j <= R; j += p) {
                isPrime[j - L] = false;
            }
        }

        for (int64_t i = 0; i < (int)isPrime.size(); i++) {
            if (isPrime[i]) {
                int64_t p = L + i;
                int freq[10]{};
                int64_t temp = p; // Use a temp variable, don't modify p directly for the loop!
                
                while(temp > 0) {
                    freq[temp % 10]++;
                    temp /= 10;
                }

                for (int d = 0; d < 10; d++) {
                    if (freq[d] > M[d]) {
                        M[d] = freq[d];
                        N[d] = 1;
                        S[d] = p;
                    }
                    else if (freq[d] == M[d]) {
                        N[d]++;
                        S[d] += p;
                    }
                }
            }
        }
    }

    for (int d = 0; d < 10; d++) {
        cout << "Digit: " << d << ", M: " << M[d] << ", N: " << N[d] << ", S: " << S[d] << '\n';
    }

    cout << accumulate(S, S + 10, 0LL) << '\n';

    return 0;
}