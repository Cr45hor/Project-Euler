#include <iostream>

using namespace std;

int const N = 1e3;

int dp[N];

int main() {
    dp[0] = 1;
    for (int x = 1; x < N; x++) {
        for (int y = x; y < N; y++) {
            dp[y] += dp[y - x];
        }
    }
    cout << dp[100] - 1 << endl;
}