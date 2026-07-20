#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>

#include "../Z.hpp"

using namespace std;

const int MOD = 1e9;
const int PHI_MOD = 4e8;
const int N = 50000;
const int THRESHOLD = 30;

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

// ---------------------------------------------------------
// Optimization 1: O(1) Exponentiation for 2^n mod 1e9
// ---------------------------------------------------------
int p1_arr[65536];
int p2_arr[65536];

void precompute_pow2() {
    p1_arr[0] = 1;
    for (int i = 1; i < 65536; i++) {
        p1_arr[i] = (p1_arr[i - 1] * 2LL) % MOD;
    }
    int64_t multiplier = (p1_arr[65535] * 2LL) % MOD; // 2^65536
    p2_arr[0] = 1;
    for (int i = 1; i < 65536; i++) {
        p2_arr[i] = (p2_arr[i - 1] * multiplier) % MOD;
    }
}

Z fast_pow2(int64_t exp) {
    int64_t low = exp & 65535;
    int64_t high = exp >> 16;
    int64_t res = (1LL * p1_arr[low] * p2_arr[high]) % MOD;
    return Z(res);
}

// ---------------------------------------------------------
// Optimization 2 & 3: 1D Combinations & Transition Cache
// ---------------------------------------------------------
vector<Z> get_pascal_row(int n) {
    vector<Z> row(n + 1, Z(0));
    row[0] = Z(1);
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j > 0; --j) {
            row[j] = row[j] + row[j - 1];
        }
    }
    return row;
}

struct Transition {
    int mult_mod;
    bool mult_big;
    Z choices;
};

int freq[16];
vector<pair<int, int>> freq_pairs;
vector<vector<Transition>> transitions;

void L() {
    for (int p : primes) {
        int k = 1;
        int64_t pk = p;
        while (pk * p <= N) {
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

    // Precalculate all state transitions to avoid loops inside DFS
    transitions.resize(freq_pairs.size());
    for (size_t id = 0; id < freq_pairs.size(); ++id) {
        int k = freq_pairs[id].first;
        int f = freq_pairs[id].second;
        
        vector<Z> row = get_pascal_row(f); // Compute only the row we need
        transitions[id].resize(f + 1);
        
        for (int i = 0; i <= f; ++i) {
            int64_t mult_val = 1;
            bool big = false;
            
            // k^i
            for (int j = 0; j < i; ++j) {
                mult_val *= k;
                if (mult_val >= THRESHOLD) big = true;
                mult_val %= PHI_MOD;
            }
            // (k + 1)^(f - i)
            for (int j = 0; j < f - i; ++j) {
                mult_val *= (k + 1);
                if (mult_val >= THRESHOLD) big = true;
                mult_val %= PHI_MOD;
            }
            
            transitions[id][i] = {(int)mult_val, big, row[i]};
        }
    }
}

// ---------------------------------------------------------
// DFS Execution
// ---------------------------------------------------------
Z res = 0;

void HL(int id, int sign, int cur_n, bool cur_big, Z cur_prod) {
    if (id == (int)freq_pairs.size()) {
        int64_t final_exp = cur_n;
        if (cur_big) {
            final_exp += PHI_MOD; // Generalized Euler's correction
        }
        
        Z term = fast_pow2(final_exp) * cur_prod;
        if (sign == 1) {
            res = res + term;
        } else {
            res = res - term;
        }
        return;
    }

    int f = freq_pairs[id].second;
    for (int i = 0; i <= f; i++) {
        const auto& trans = transitions[id][i];
        
        // Advance state using precalculated multipliers
        int64_t next_n = (1LL * cur_n * trans.mult_mod) % PHI_MOD;
        bool next_big = cur_big || trans.mult_big || (1LL * cur_n * trans.mult_mod >= THRESHOLD);
        
        HL(id + 1, sign * (i % 2 == 0 ? 1 : -1), next_n, next_big, cur_prod * trans.choices);
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    sieve();
    precompute_pow2();
    L();
    
    HL(0, 1, 1, false, Z(1));
    
    cout << res << '\n';
    return 0;
}