#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdint>

using namespace std;

int const N = 64e6;

int spf[N];

void sieve() {
    for (int i = 1; i < N; i++) {
        spf[i] = i;
    }

    for (int i = 4; i < N; i += 2) {
        spf[i] = 2;
    }

    for (int i = 3; i * i < N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int64_t sigma_2[N];

int64_t geometric_sum(int64_t d, int k) {
    assert(k < 64);
    int64_t sum = 1, base = 1;
    for (int i = 1; i <= k; i++) {
        base *= d;
        sum += base;
    }
    return sum;
}

bool is_square(int64_t x) {
    int64_t s = sqrt(x);
    return s * s == x;
}

int main() {
    sieve();

    sigma_2[1] = 1;
    for (int n = 2; n < N; n++) {
        int p = spf[n];
        int q = n;
        int k = 0;
        while(q % p == 0) {
            q /= p;
            k++;
        }
        sigma_2[n] = sigma_2[q] * geometric_sum(1LL * p * p, k);
    }

    int64_t res = 0;
    for (int n = 1; n < N; n++) {
        if (is_square(sigma_2[n])) {
            res += n;
        }
    }
    cout << res << '\n';

    return 0;
}