#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

bool minimize(int64_t &a, int64_t b) {
    if (b < a) {
        a = b;
        return true;
    }
    return false;
}

struct generator {
    int t = 0;
    const int a = 615949;
    const int b = 797807;
    const int mask = (1 << 20) - 1;
    generator() {}

    int curr() {
        return t;
    }

    int next() {
        t = (1LL * t * a + b) & mask;
        return t;
    }
};

const int N = 1000;
const int offset = 1 << 19;

int s[N][N];
int64_t sum[N][N + 1];

int main() {
    generator g{};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            s[i][j] = g.next() - offset;
        }
    }

    for (int i = 0; i < N; i++) {
        sum[i][0] = 0;
        for (int j = 0; j <= i; j++) {
            sum[i][j + 1] = sum[i][j] + s[i][j];
        }
    }

    auto query = [&](int row, int l, int r) ->int64_t {
        return sum[row][r] - sum[row][l - 1];
    };

    int64_t ans = INT64_MAX;
    for (int i = 0; i < N; i++) {
        for (int j = 1; j <= i + 1; j++) {
            int64_t prefix_sum = 0;
            for (int k = 0; k + i < N; k++) {
                int64_t val = query(i + k, j, j + k);
                prefix_sum += val;
                minimize(ans, prefix_sum);
            }
        }
    }
    cout << ans << '\n';
}