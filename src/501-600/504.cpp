#include <iostream>
#include <numeric>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <cmath>

using namespace std;

int const M = 100;

int g[M + 1][M + 1];

void precompute() {
    for (int i = 1; i <= M; ++i) {
        for (int j = 1; j <= M; ++j) {
            g[i][j] = gcd(i, j);
        }
    }
}

// Pick's theorem
int64_t I(int a, int b, int c, int d) {
    int64_t A2 = 1LL * a * b + 1LL * b * c + 1LL * c * d + 1LL * d * a;
    int64_t B = g[a][b] + g[b][c] + g[c][d] + g[d][a];
    assert((A2 - B) % 2 == 0);
    return (A2 + 2 - B) / 2;
}

bool is_square(int64_t x) {
    int64_t r = (int64_t)(sqrt(x) + 0.5);
    return r * r == x;
}

int main() {
    precompute();

    int res = 0;
    for (int a = 1; a <= M; ++a) {
        for (int b = 1; b <= M; ++b) {
            for (int c = 1; c <= M; ++c) { 
                for (int d = 1; d <= M; d++) {
                    res += is_square(I(a, b, c, d));
                }
            }
        }
    }
    cout << res << '\n';
}