#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

int const N = 1e4;

bool is_prime[N];
vector<int> primes;

void sieve() {
    for (int i = 2; i < N; i++) {
        is_prime[i] = true;
    }

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

    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}

int const MAXX = 5e7;

vector<int> prime_power[5];
bool expressible[MAXX];

int main() {
    sieve();

    prime_power[1] = primes;
    for (int p: primes) {
        int pk = p;
        for (int k = 2; k < 5; k++) {
            if (1LL * pk * p >= 1LL * MAXX) {
                break;
            }
            pk *= p;
            prime_power[k].push_back(pk);
        }
    }

    for (int p2: prime_power[2]) {
        for (int q3: prime_power[3]) {
            if (p2 + q3 >= MAXX) {
                break;
            }
            for (int r4: prime_power[4]) {
                int sum = p2 + q3 + r4;
                if (sum >= MAXX) {
                    break;
                }
                expressible[sum] = true;
            }
        }
    }

    cout << count(expressible, expressible + MAXX, true) << '\n';
}