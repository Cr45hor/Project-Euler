#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <cmath>

using namespace std;

// Precompute small primes once to drastically speed up segmented sieves
vector<int> primes;

void precompute_primes() {
    const int MAX_PRIME = 5200000;
    vector<char> is_p(MAX_PRIME + 1, true);
    is_p[0] = is_p[1] = false;
    for (int p = 2; p * p <= MAX_PRIME; p++) {
        if (is_p[p]) {
            for (int i = p * p; i <= MAX_PRIME; i += p) {
                is_p[i] = false;
            }
        }
    }
    for (int p = 2; p <= MAX_PRIME; p++) {
        if (is_p[p]) primes.push_back(p);
    }
}

vector<char> segmentedSieve(int64_t L, int64_t R) {
    vector<char> isPrime(R - L + 1, true);
    if (L == 1) isPrime[0] = false;
    int64_t lim = sqrt(R);
    
    for (int p : primes) {
        if (p > lim) break;
        int64_t start = max(1LL * p * p, (L + p - 1) / p * p);
        for (int64_t j = start; j <= R; j += p) {
            isPrime[j - L] = false;
        }
    }
    return isPrime;
}

int64_t S(int n) {
    int64_t L = 1LL * (n - 2) * (n - 3) / 2 + 1;
    int64_t R = 1LL * (n + 2) * (n + 3) / 2;
    vector<char> isPrime = segmentedSieve(L, R);

    int cols = n + 3;
    vector<int> par(5 * cols, -1);

    // Standard DSU with path compression
    auto root = [&](auto& self, int x) -> int {
        return par[x] < 0 ? x : par[x] = self(self, par[x]);
    };

    // Union by size
    auto merge = [&](int x, int y) {
        x = root(root, x);
        y = root(root, y);
        if (x == y) return false;
        
        // Ensure x is the larger component
        if (par[x] > par[y]) swap(x, y); 
        par[x] += par[y];
        par[y] = x;
        return true;
    };

    auto mapping = [&](int r, int c) {
        return r * cols + c;
    };

    // Cache start indices for the 5 relevant rows
    int64_t row_start_cache[5];
    for (int r = 0; r < 5; ++r) {
        int64_t row_order = n - 2 + r;
        row_start_cache[r] = 1LL * row_order * (row_order - 1) / 2;
    }

    auto is_prime = [&](int r, int c) {
        if (r < 0 || r >= 5) return false;
        int row_order = n - 2 + r;
        if (c < 0 || c >= row_order) return false;
        int64_t i = row_start_cache[r] + 1 + c;
        return (bool)isPrime[i - L];
    };

    // Process using 4 backward connectivity directions to cover all edges exactly once
    int dr[] = {-1, -1, -1, 0};
    int dc[] = {-1, 0, 1, -1};

    for (int r = 0; r < 5; r++) {
        int row_order = n - 2 + r;
        for (int c = 0; c < row_order; c++) {
            if (!is_prime(r, c)) continue;
            int u = mapping(r, c);
            
            for (int dir = 0; dir < 4; dir++) {
                int nr = r + dr[dir];
                int nc = c + dc[dir];
                if (is_prime(nr, nc)) {
                    merge(u, mapping(nr, nc));
                }
            }
        }
    }

    int64_t res = 0;
    int r = 2; // relative index for the n-th row
    int row_order = n;
    
    for (int c = 0; c < row_order; c++) {
        if (is_prime(r, c)) {
            int p = root(root, mapping(r, c));
            if (-par[p] >= 3) {
                res += row_start_cache[r] + 1 + c;
            }
        }
    }

    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    precompute_primes();
    
    assert(S(10000) == 950007619);
    cout << (S(5678027) + S(7208785)) << '\n';
    
    return 0;
}