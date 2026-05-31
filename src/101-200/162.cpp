#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;

int const N = 16;

int mapping(int d) {
    if (d == 0) return 1;
    if (d == 1) return 2;
    if (d == 10) return 4;
    return 0;
}

uint64_t dp[N][8][2];
int vis[N][8][2];

uint64_t calc(int n, int mask, int lead_zero) {
    if (n >= N) {
        return mask == 7;
    }

    uint64_t& res = dp[n][mask][lead_zero];
    if (vis[n][mask][lead_zero]) {
        return res;
    }
    vis[n][mask][lead_zero] = 1;

    for (int d = 0; d < 16; d++) {
        int next_lead_zero = lead_zero && (d == 0);
        int next_mask = (next_lead_zero ? 0 : mask | mapping(d));
        res += calc(n + 1, next_mask, next_lead_zero);
    }

    return res;
}

int main() {
    uint64_t res = calc(0, 0, 1);
    // Print them in hexadecimal to verify the digits
    // With uppercase letters, the digits are 1, 2, 4, A, B, C, D, E, F
    cout << hex << uppercase << res << '\n';
    return 0;
}