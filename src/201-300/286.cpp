#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdio>
#include <iomanip>

using namespace std;

using ld = long double;

ld const eps = 1e-12;

ld f(ld q) {
    ld prob_score[51];
    for (int x = 0; x <= 50; x++) {
        prob_score[x] = (ld)(q - x)/q;
    }

    ld dp[51][51]{};
    dp[0][0] = 1;   

    for (int x = 1; x <= 50; x++) {
        dp[x][0] = dp[x - 1][0] * ((ld)1 - prob_score[x]);
        dp[x][x] = dp[x - 1][x - 1] * prob_score[x];
        for (int score = 1; score < x; score++) {
            dp[x][score] = dp[x - 1][score] * ((ld)1 - prob_score[x]) + dp[x - 1][score - 1] * prob_score[x];
        }
    }

    return dp[50][20];
}

int main() {
    ld L = 52, R = 53;
    while(R - L > eps) {
        ld mid = (ld)(L + R)/2;
        if (f(mid) < (ld)0.02) {
            R = mid;
        }
        else {
            L = mid;
        }
    }
    cout << setprecision(10) << fixed << L;
}