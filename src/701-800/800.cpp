#include <iostream>
#include <cmath>

using namespace std;

int const N = 2e7;

int is_prime[N];
int primes[N];
int M = 0;

void sieve() {
    for (int i = 1; i < N; i++) {
        is_prime[i] = 1;
    }
    is_prime[0] = is_prime[1] = 0;
    for (int i = 4; i < N; i += 2) {
        is_prime[i] = 0;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += 2 * i) {
                is_prime[j] = 0;
            }
        }
    }

    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes[M++] = i;
        }
    }
}

long double f(int p, int q) {
    return p * log(q) + q * log(p);
}

int main() {
    sieve();
    long double const target = log(800800) * 800800;

    long long res = 0;
    int max_q = primes[M - 1];
    int max_q_index = M - 1;
    for (int i = 0; i < M; i++) {
        int p = primes[i];
        while (max_q_index > 0 && f(p, max_q) > target) {
            max_q = primes[--max_q_index];
        }
        if (f(p, max_q) > target || i >= max_q_index) {
            break;
        }
        res += max_q_index - i;
    }
    cout << res << '\n';
}