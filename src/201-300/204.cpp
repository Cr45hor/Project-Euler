#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <cstdint>
#include <cassert>

using namespace std;

int const N = 25;
int const MAXX = 1e9;

int primes[N] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
map<int, int> dp[N];

uint64_t calc(int n, int id) {
    if (n < 2) {
        return 1;
    }

    if (id == 0) {
        // Number of power of 2 that are at most n
        int msb = 31 - __builtin_clz(n);
        return msb + 1;
    }

    if (dp[id].count(n)) {
        return dp[id][n];
    }

    uint64_t res = calc(n, id - 1);
    int p = primes[id];
    int pk = 1;
    for (int k = 1; ; k++) {
        if (1LL * pk * p > n) {
            break;
        }
        pk *= p;
        res += calc(n / pk, id - 1);
    }
    dp[id][n] = res;
    return res;
}

int main() {
    cout << calc(MAXX, N - 1) << '\n';
    return 0;
}