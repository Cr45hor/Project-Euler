#include <iostream>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <string>

using namespace std;

int const MAX_LENGTH = 19;
int const MAX_POWER = 18;

uint64_t pow11[MAX_POWER];
int pow11_mask[MAX_POWER];

void precompute_pow11() {
    pow11[0] = 11;
    for (int i = 1; i < MAX_POWER; ++i) {
        pow11[i] = pow11[i - 1] * 11;
    }

    cout << pow11[10] << ' ' << pow11[12] << '\n';
    cout << pow11[0] << ' ' << pow11[1] << '\n';
    for (int i = 0; i < MAX_POWER; i++) {
        pow11_mask[i] = 1LL << i;
    }
    pow11_mask[10] |= pow11_mask[0];
    pow11_mask[10] |= pow11_mask[1];
}

uint64_t dp[MAX_LENGTH][1 << (MAX_POWER + 1)];

void precompute_dp() {
    for (int length = 0; length < MAX_LENGTH; ++length) {
        for (int mask = 0; mask < (1 << (MAX_POWER + 1)); ++mask) {
            dp[length][mask] = 0;
        }
    }

    // dp[]
}

void precompute() {
    precompute_pow11();
    // precompute_dp();
}

int smaller_update(int smaller, const string& n_str, int pos, int k) {
    if (smaller) {
        return smaller;
    }

    return 1;
}

uint64_t E(uint64_t n) {
    if (n <= 10) {
        return n;
    }

    string n_str = to_string(n);
    int length = n_str.size();

    return 0;
}

int main() {
    precompute();

    // uint64_t target = 200LL;

    // uint64_t L = 1, R = target * 2;
    // while(L < R) {
    //     uint64_t mid = L + (R - L) >> 1;
    //     if (E(mid) >= target) {
    //         R = mid;
    //     } else {
    //         L = mid + 1;
    //     }
    // }
    // cout << L << '\n';
    return 0;
}