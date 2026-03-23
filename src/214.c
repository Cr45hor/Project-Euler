#include <stdio.h>
#include <stdint.h>

int const N = 4e7;

int phi[N];
void sieve() {
    for (int x = 1; x < N; x++) {
        phi[x] = x;
    }

    for (int p = 2; p < N; p++) {
        if (phi[p] == p) {
            phi[p] = p - 1;
            for (int q = p * 2; q < N; q += p) {
                phi[q] = (phi[q] / p) * (p - 1);
            }
        }
    }
}

int lvl[N];

int main() {
    sieve();

    lvl[1] = 1;
    for (int x = 2; x < N; x++) {
        lvl[x] = lvl[phi[x]] + 1;
    }

    int64_t res = 0;
    for (int x = 1; x < N; x++) {
        if (lvl[x] == 25 && phi[x] == x - 1) {
            res += x;
        }
    }

    printf("%lld\n", res);

    return 0;
}