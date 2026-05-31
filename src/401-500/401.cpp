#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;

int const MOD = 1e9;

int normalize(int x) {
    if (x >= MOD) {
        x -= MOD;
    }
    else if (x < 0) {
        x += MOD;
    }
    return x;
}

int add(int x, int y) { return normalize(x + y); }
int sub(int x, int y) { return normalize(x - y); }
int mul(int x, int y) { return (1LL * x * y) % MOD; }

long long const N = 1e15;

int sum_square(long long n) {
    long long factor[3] = {n, 2 * n + 1, n + 1};
    int r2 = n % 2;
    int r3 = n % 3;
    factor[r2 * 2] >>= 1;
    factor[r3] /= 3;
    return mul(mul(factor[0] % MOD, factor[1] % MOD), factor[2] % MOD);
}

int sum_square(long long L, long long R) {
    return sub(sum_square(R), sum_square(L - 1));
}
 
int main() {
    // // \sum_{d = 1}^{N} d^2 \cdot \lfloor \frac{N}{d} \rfloor
    int const sqrt_N = (int)sqrt(N);
    int res = 0;

    for (int d = 1; d <= sqrt_N; d++) {
        int tmp_floor = (N / d) % MOD;
        int prod = mul(mul(d, d), tmp_floor);
        res = add(res, prod);
    }

    assert(N / (sqrt_N + 1) < sqrt_N + 1);
    for (int e = 1; e <= sqrt_N; e++) {
        long long L = N / (e + 1) + 1;
        long long R = N / e;
        res = add(res, mul(sum_square(L, R), e));
    }

    if (1LL * sqrt_N * sqrt_N == N) {
        res = sub(res, mul(N % MOD, sqrt_N));
    }
    cout << res << '\n';

    return 0;
}