#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>

#include "../Z.hpp"
const int MOD = 1e9;
const int PHI_MOD = 4e8;

using namespace std;

int const N = 50000;
int const M = 6000;

bool is_prime[N];
vector<int> primes;

void sieve() {
    is_prime[2] = true;
    for (int i = 3; i < N; i += 2) {
        is_prime[i] = true;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i * 2) {
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

Z C[M][M];

void precompute() {
    C[0][0] = Z(1);
    for (int n = 1; n < M; n++) {
        C[n][0] = C[n][n] = Z(1);
        for (int k = 1; k < n; k++) {
            C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
        }
    }
}

int freq[16];
vector<pair<int, int>> freq_pairs;

void L() {
    for (int p: primes) {
        // Find the largest power of p that is <= N
        int k = 1;
        long long pk = p;
        while(pk * p <= N) {
            pk *= p;
            k++;
        }
        assert(k <= 15);
        freq[k]++;
    }

    for (int k = 1; k <= 15; k++) {
        if (freq[k] > 0) {
            freq_pairs.emplace_back(k, freq[k]);
        }
    }
}

Z offset = Z(2).pow(PHI_MOD);

struct expo_state {
    int n;
    bool bigger;
    static const int THRESHOLD = 30;

    expo_state(int __n = 1, bool __bigger = false): n(__n), bigger(__bigger) {}

    expo_state multi(int k, int f) const {
        // Multiply the current state by k^f and mod it by PHI_MOD
        int factor = binpow(k, f, PHI_MOD);
        int new_n = (int64_t)n * factor % PHI_MOD;
        bool new_bigger = bigger || (factor > THRESHOLD);
        if (!new_bigger) {
            // Check if n * k^f exceeds THRESHOLD
            // Compute n, n * k, n * k^2, ..., n * k^f and check if any exceeds THRESHOLD
            int64_t temp = n;
            for (int i = 0; i < f; i++) {
                temp *= k;
                if (temp > THRESHOLD) {
                    new_bigger = true;
                    break;
                }
            }
        }
        return expo_state(new_n, new_bigger);
    }

    Z eval() const {
        // Return 2^n mod MOD
        if (!bigger) {
            return Z(2).pow(n);
        }
        else {
            return offset * Z(2).pow(n);
        }
    }
};

Z res = 0;
void HL(int id, int sign, expo_state state, Z prod) {
    if (id == (int)freq_pairs.size()) {
        res += state.eval() * prod * sign;
        return;
    }

    auto [k, f] = freq_pairs[id];
    for (int i = 0; i <= f; i++) {
        Z choices = C[f][i];
        // k^i * (k + 1)^(f - i)
        expo_state next_state = state.multi(k, i).multi(k + 1, f - i);
        HL(id + 1, sign * (i % 2 == 0 ? 1 : -1), next_state, prod * choices);
    }
}

int main() {
    sieve();
    precompute();
    L();
    HL(0, 1, expo_state(1, false), Z(1));
    cout << res << '\n';
    return 0;
}