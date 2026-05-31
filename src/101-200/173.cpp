#include <iostream>

using namespace std;

int const N = 1e6;
int const M = N >> 2;

int d[M + 1];

void sieve() {
    for (int i = 1; i <= M; i++) {
        for (int j = i; j <= M; j += i) {
            d[j]++;
        }
    }
}

int main() {
    sieve();

    long long res = 0;
    for (int s = 8; s <= N; s += 4) {
        res += 1LL * d[s >> 2] / 2;
    }
    cout << res << '\n';

    return 0;
}