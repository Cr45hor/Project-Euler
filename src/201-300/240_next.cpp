#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <numeric>

using namespace std;

int64_t factorial(int n) {
    int64_t f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

int64_t C(int n, int k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    int64_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }
    return res;
}

int64_t binpow(int a, int n) {
    int64_t res = 1;
    int64_t base = a;
    while (n > 0) {
        if (n & 1) res *= base;
        base *= base;
        n >>= 1;
    }
    return res;
}

int64_t count_perm(int n, vector<int> counts) {
    assert(accumulate(counts.begin(), counts.end(), 0) == n);
    int64_t perms = 1;
    for (int c : counts) {
        if (c > 0) {
            perms *= C(n, c);
            n -= c;
        }
    }
    return perms;
}

int main() {
    int const S = 70, side = 12, N = 10, M = 20;
    assert(M >= N);

    int total_positions = N + side - 1;
    vector<int> stars_bars(total_positions, 0);

    for (int i = total_positions - N; i < total_positions; ++i) {
        stars_bars[i] = 1;
    }

    int64_t res = 0;
    do {
        vector<int> counts(side + 1, 0);
        int current_face = 1;
        
        for (int i = 0; i < total_positions; ++i) {
            if (stars_bars[i] == 1) counts[current_face]++;
            else current_face++;
        }

        int current_sum = 0;
        for (int i = 1; i <= side; ++i) {
            current_sum += i * counts[i];
        }

        if (current_sum == S) {
            int min_face = find_if(counts.begin() + 1, counts.end(), [](int c) { return c > 0; }) - counts.begin();
            for (int s = N; s <= M; s++) {
                res += C(M, s) * count_perm(s, counts) * binpow(min_face - 1, M - s);
                counts[min_face]++;
            }
        }

    } while (next_permutation(stars_bars.begin(), stars_bars.end()));

    cout << res << "\n";
    return 0;
}