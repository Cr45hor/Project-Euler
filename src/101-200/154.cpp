#include <iostream>
#include <cassert>
#include <cstdint>

using namespace std;

int const N = 2e6;

int v2_fact[N + 1];
int v5_fact[N + 1];

void precompute(int p, int v_fact[]) {
    for (int x = 1; x <= N; x++) {
        v_fact[x] = v_fact[x - 1];
        int y = x;
        while(y % p == 0) {
            y /= p;
            v_fact[x]++;
        }
    }
}

int v10_Cnij(int n, int i, int j) {
    assert(0 <= i && i <= n && 0 <= j && j <= n - i && n <= N);
    int v2 = v2_fact[n] - v2_fact[i] - v2_fact[j] - v2_fact[n - i - j];
    int v5 = v5_fact[n] - v5_fact[i] - v5_fact[j] - v5_fact[n - i - j];
    return min(v2, v5);
}

int main() {
    precompute(2, v2_fact);
    precompute(5, v5_fact);

    int64_t res = 0;

    #pragma omp parallel for reduction(+:res)
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N - i; j++) {
            res += (v10_Cnij(N, i, j) >= 12);
        }
    }
    cout << res << '\n';
    return 0;
}