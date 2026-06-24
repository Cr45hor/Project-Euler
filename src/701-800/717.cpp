#include <iostream>
#include <cassert>
#include <cstdint>
#include <algorithm>

using namespace std;

int32_t power(int64_t a, int64_t n, int32_t MOD) {
    int32_t res = 1;
    a %= MOD;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = 1LL * res * a % MOD;
        }
        a = 1LL * a * a % MOD;
    }
    return res;
}

int32_t g(int32_t p) {
    assert(p >= 3);
}

int main() {
    cout << power(2, 4, 10000) << '\n';
    cout << power(2, 5, 10000) << '\n';
    cout << power(2, 6, 10000) << '\n';
}