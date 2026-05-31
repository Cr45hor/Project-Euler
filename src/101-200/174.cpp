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

int L[N + 1];

int main() {
    sieve();

    for (int s = 8; s <= N; s += 4) {
        L[s] = d[s >> 2] / 2;
    }

    int cnt = 0;
    for (int s = 8; s <= N; s += 4) {
        cnt += (L[s] <= 10);
    }
    cout << cnt << '\n';

    return 0;
}