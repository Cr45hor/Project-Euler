#include <iostream>
#include <cstdint>

using namespace std;

const int64_t MOD = 1234567891;
const int64_t PHI = MOD - 1;
const int64_t N = 1e14;

// Cấu trúc để tính nhanh 2^E mod MOD trong O(1)
const int STEP = 35137; // xấp xỉ sqrt(MOD)
int64_t pow_small[STEP + 1];
int64_t pow_large[STEP + 1];

void precompute_pow() {
    pow_small[0] = 1;
    for (int i = 1; i <= STEP; ++i) 
        pow_small[i] = (pow_small[i - 1] * 2) % MOD;
    
    int64_t giant_step = pow_small[STEP];
    pow_large[0] = 1;
    for (int i = 1; i <= STEP; ++i)
        pow_large[i] = (pow_large[i - 1] * giant_step) % MOD;
}

// Truy vấn 2^E mod MOD trong O(1)
int64_t fast_pow2(int64_t exp) {
    exp %= PHI; // Định lý Fermat nhỏ
    if (exp < 0) exp += PHI;
    return (pow_large[exp / STEP] * pow_small[exp % STEP]) % MOD;
}

int64_t range_sum(int64_t L, int64_t R) {
    int64_t factor[2] = {L + R, R - L + 1};
    int r = factor[0] & 1;
    factor[r] >>= 1;
    return ((factor[0] % MOD) * (factor[1] % MOD)) % MOD;
}

int main() {
    precompute_pow();

    int64_t ans = 0;
    int64_t all_subsets = fast_pow2(N - 1);

    for (int64_t L = 1, R; L <= N; L = R + 1) {
        int64_t k = N / L;
        R = N / k;

        int64_t s = range_sum(L, R);
        
        // t = (2^(N-1) - 2^(N-k)) % MOD
        int64_t pow_nk = fast_pow2(N - k);
        int64_t t = (all_subsets - pow_nk + MOD) % MOD;

        ans = (ans + (s * t)) % MOD;
    }

    cout << ans << '\n';

    return 0;
}