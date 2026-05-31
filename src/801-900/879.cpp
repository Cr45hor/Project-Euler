#include <iostream>
#include <cassert>
#include <cstdint>

using namespace std;

bool bit(int64_t mask, int k) {
    return (mask >> k) & 1;
}

int const N = 3;
int const NN = N * N;

int mapping(int i, int j) {
    assert(0 <= i && i < N);
    assert(0 <= j && j < N);
    return i * N + j;
}

pair<int, int> rev_mapping(int coor) {
    assert(0 <= coor && coor < NN);
    return {coor / N, coor % N};
}

int mask_path(int i1, int j1, int i2, int j2) {
    assert(0 <= i1 && i1 < N);
    assert(0 <= j1 && j1 < N);
    assert(0 <= i2 && i2 < N);
    assert(0 <= j2 && j2 < N);

    int mask = (1 << mapping(i1, j1)) | (1 << mapping(i2, j2));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            bool collinear = ((i2 - i1) * (j - j1) == (j2 - j1) * (i - i1));
            bool between = (min(i1, i2) <= i && i <= max(i1, i2) && min(j1, j2) <= j && j <= max(j1, j2));
            if (collinear && between) {
                mask |= (1 << mapping(i, j));
            }
        }
    }
    
    return mask;
}

int64_t dp[1 << NN][NN];
int64_t path[NN][NN];

void precompute() {
    for (int i1 = 0; i1 < N; i1++) {
        for (int j1 = 0; j1 < N; j1++) {
            for (int i2 = 0; i2 < N; i2++) {
                for (int j2 = 0; j2 < N; j2++) {
                    path[mapping(i1, j1)][mapping(i2, j2)] = mask_path(i1, j1, i2, j2);
                }
            }
        }
    }
}

int main() {
    precompute();

    for (int u = 0; u < NN; u++) {
        dp[1 << u][u] = 1;
    }

    int64_t passwords = 0;
    for (int mask = 1; mask < (1 << NN); mask++) {
        for (int u = 0; u < NN; u++) {
            if (!bit(mask, u) || dp[mask][u] == 0) {
                continue;
            }
            if (__builtin_popcount(mask) >= 2) {
                passwords += dp[mask][u];
            }
            for (int v = 0; v < NN; v++) {
                if (bit(mask, v)) {
                    continue;
                }
                assert((mask | path[u][v]) > mask);
                int num_new_bits = __builtin_popcount(path[u][v] & ~mask);
                if (num_new_bits == 1) {
                    dp[mask | path[u][v]][v] += dp[mask][u];
                }
            }
        }
    }

    cout << "Total passwords: " << passwords << '\n';
    return 0;
}