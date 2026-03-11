#include <iostream>
#include <algorithm>

using namespace std;

int const N = 1e5 + 6;

int is_prime[N];
void sieve() {
    is_prime[2] = true;
    for (int x = 3; x < N; x += 2) {
        is_prime[x] = true;
    }
    for (int x = 3; x * x < N; x += 2) {
        if (is_prime[x]) {
            for (int y = x * x; y < N; y += x) {
                is_prime[y] = false;
            }
        }
    }
}

int dp[N];

int main() {
    sieve();

    dp[0] = 1;

    for (int p = 2; p < N; p++) {
        if (!is_prime[p]) {
            continue;
        }

        for (int x = p; x < N; x++) {
            dp[x] += dp[x - p];
        }
    }

    for (int x = 0; x < N; x++) {
        if (dp[x] > 5000) {
            cout << "found " << ' ' << x << ' ' << dp[x] << '\n';
            return 0;
        }
    }
}