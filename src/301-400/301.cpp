#include <iostream>

using namespace std;

int const N = 4;

int dp[N][2];

int main() {
    dp[0][0] = dp[0][1] = 1;
    for (int i = 1; i < N; i++) {
        dp[i][0] = dp[i - 1][0] + dp[i - 1][1];
        dp[i][1] = dp[i - 1][0];
        
    }
}