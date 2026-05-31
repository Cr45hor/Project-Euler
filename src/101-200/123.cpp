#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

int64_t const THRESHOLD = 1e10;
int const N = 1e6;

bool is_prime[N];
vector<int> primes;

void sieve() {
    fill(is_prime, is_prime + N, true);
    is_prime[0] = is_prime[1] = false;
    
    for (int i = 4; i < N; i += 2) {
        is_prime[i] = false;
    }

    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += 2 * i) {
                is_prime[j] = false;
            }
        }
    }

    primes.push_back(1);
    for (int i = 2; i < N; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}

int main() {
    sieve();

    for (int i = 1; i < (int)primes.size(); i += 2) {
        int64_t r = (2LL * i % primes[i]) * primes[i];
        if (r > THRESHOLD) {
            cout << i << '\n';
            break;
        }
    }
}