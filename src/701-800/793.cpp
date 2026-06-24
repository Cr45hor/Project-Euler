#include <iostream>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <vector>

using namespace std;

const int MOD = 50515093;
const int64_t seed = 290797;

struct PRNG {
    int s;
    int m;

    PRNG(int s0, int modulo) : s(s0), m(modulo) {}
    
    int curr() {
        return s;
    }

    int next() {
        s = (int64_t(s) * s) % m;
        return s;
    }
};

int const N = 1e6 + 3;
int S[N];

void prepare() {
    PRNG prng(seed, MOD);
    for (int i = 0; i < N; i++) {
        S[i] = prng.curr();
        prng.next();
    }
}

int64_t M(int n) {
    assert(n <= N);
    sort(S, S + n);

    auto cnt = [&](int64_t x, int64_t y) -> int {
        // Count numbers in S that are in the range [x, y]
        if (x > y) return 0;
        return upper_bound(S, S + n, y) - lower_bound(S, S + n, x);
    };

    auto query = [&](int64_t cap) -> int64_t {
        int64_t pairs = 0;
        for (int i = 0; i < n; i++) {
            if (S[i] > cap) {
                break;
            }
            pairs += cnt(S[i] + 1, cap / S[i]);
        }
        return pairs;
    };

    int64_t L = 0, R = 1LL * MOD * MOD;
    int64_t tot_pairs = (int64_t)n * (n - 1) / 2;
    int64_t goal = (tot_pairs + 1) / 2;

    while(L < R) {
        int64_t mid = (L + R) / 2;
        if (query(mid) >= goal) {
            R = mid;
        }
        else {
            L = mid + 1;
        }
    }
    return L;
}

int main() {
    prepare();
    cout << M(N) << '\n';
}