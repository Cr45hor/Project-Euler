#include <iostream>
#include <assert.h>

using namespace std;

int const MAX_PRIME = 3e6;
bool is_prime[MAX_PRIME];

void sieve() {
    for (int i = 2; i < MAX_PRIME; i++) {
        is_prime[i] = true;
    }
    for (int i = 2; i * i < MAX_PRIME; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < MAX_PRIME; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

bool primality_test(int n) {
    assert(abs(n) < MAX_PRIME);
    if (n < 0) {
        return 0;
    }
    return is_prime[n];
}

int f(int n, int a, int b) {
    return n * n + a * n + b;
}

int produce(int a, int b) {
    int n = 0;
    while (primality_test(f(n, a, b))) {
        n++;
    }
    return n;
}

int main() {
    sieve();

    pair<int, int> best = {-1, 0};
    for (int a = -1000; a <= 1000; a++) {
        for (int b = -1000; b <= 1000; b++) {
            best = max(best, {produce(a, b), a * b});
        }
    }

    cout << best.second << '\n';
    return 0;
}