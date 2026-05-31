#include <iostream>
#include <cmath>
#include <numeric>
#include <cstdint>
#include <iomanip>

using namespace std;

int const M = 24690;
int const N = 12345;

double_t dp[2][M + 1];
int row_id = 0;

double C_ratio(int r, int b) {
    return 1.0 * r * (r - 1) / (1.0 * (r + b) * (r + b - 1));
}

double cross_ratio(int r, int b) {
    return 2.0 * r * b / (1.0 * (r + b) * (r + b - 1));
}

int main() {
    for (int b = 1; b <= N; b++){
        row_id ^= 1;
        dp[row_id][0] = 1;
        for (int r = 2; r <= M; r += 2) {
            dp[row_id][r] = (cross_ratio(r, b) * dp[row_id ^ 1][r] + C_ratio(r, b) * dp[row_id][r - 2]) / (1.0 - C_ratio(b, r));
        }
    }
    cout << setprecision(10) << fixed << dp[row_id][M] << '\n';
}