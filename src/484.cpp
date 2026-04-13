#include <iostream>
#include <stdint.h>

using namespace std;

int64_t const N = 5e15;
int64_t const MAX_PRIME = 70710678; // floor(sqrt(N))

int spf[MAX_PRIME + 1];
int primes[MAX_PRIME + 1];
int PRIME_COUNT = 0;

void sieve() {
    for (int i = 2; i <= MAX_PRIME; i++) {
        spf[i] = i;
    }
    for (int i = 2; i * i <= MAX_PRIME; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_PRIME; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    for (int i = 2; i <= MAX_PRIME; i++) {
        if (spf[i] == i) {
            primes[PRIME_COUNT++] = i;
        }
    }
}

int64_t f(int64_t pk, int p, int k) {
    return (k % p == 0 ? pk : pk / p);
}

int64_t ans = N - 1;
void dfs(int prime_index, int64_t current_d, int64_t current_g) {
    // Loop over primes
    for (int i = prime_index; i < PRIME_COUNT; i++) {
        int64_t p = primes[i];
        
        // If p^2 * current_d > N, no further primes will work
        if (current_d > N / (p * p)) {
            break;
        }

        int64_t pk = p;
        for (int k = 2; ; k++) {
            // Guard against pk * p overflowing int64_t
            if (N / current_d / p < pk) {
                break;
            }
            pk *= p;
            
            int64_t new_d = current_d * pk;
            int64_t g_pk = f(pk, p, k) - f(pk / p, p, k - 1);
            
            // Only branch if g_pk actually contributes to the sum
            if (g_pk != 0) {
                int64_t new_g = current_g * g_pk;
                ans += new_g * (N / new_d);
                dfs(i + 1, new_d, new_g);
            }
        }
    }
}

int main() {
    sieve();
    dfs(0, 1, 1);
    cout << ans << endl;
    return 0;
}