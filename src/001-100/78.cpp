#include <iostream>
#include <algorithm>

using namespace std;

int const N = 1e5;

long long dp[N];

int main() {
    dp[0] = 1;
    for (int w = 1; w < N; w++) {
        for (int S = w; S < N; S++) {
            (dp[S] += dp[S - w]) %= 1000000;
        }
        if (dp[w] == 0) {
            cout << w << '\n';
            break;
        }
    }
}