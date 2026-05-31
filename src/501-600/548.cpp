#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>

using namespace std;

int const N = 1e8;

int g[N];

int main() {
    g[1] = 1;
    for (int d = 1; d < N; d++) {
        for (int k = 2; d * k < N; k++) {
            g[d * k] += g[d];
        }
    }

    for (int i = 1; i < N; i++) {
        if (g[i] == i) {
            cout << i << ' ';
        }
    }
    cout << '\n';
}