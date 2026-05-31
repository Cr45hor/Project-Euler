#include <iostream>
#include <gmpxx.h>
#include <numeric>
#include <cstdint>

using namespace std;

int const N = 1e8;

bool valid[N];

bool prime_sieve[N];
void sieve() {
    for (int i = 3; i < N; i += 2) {
        prime_sieve[i] = true;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (prime_sieve[i]) {
            for (int j = i * i; j < N; j += 2 * i) {
                prime_sieve[j] = false;
            }
        }
    }
}

int main() {
    sieve();

    for (int i = 1; i + 1 < N; i++) {
        valid[i] = prime_sieve[i + 1] && (i % 4 != 0);
    }
    
    valid[1] = valid[2] = true;
    for (int d = 2; d < N; d++) {
        for (int k = (d & 1 ? 2 : 3); k * d < N; k += 2) {
            valid[k * d] &= prime_sieve[d + k];
        }
    }
    
    int64_t sum = 0;
    for (int n = 1; n < N; n++) {
        if (valid[n]) {
            sum += n;
        }
    }
    cout << sum << '\n';
}