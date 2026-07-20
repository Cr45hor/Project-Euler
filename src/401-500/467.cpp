#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>

#include "../Z.hpp"
const int MOD = 1e9 + 7;

using namespace std;

int digital_root(int n) {
    int r = n % 9;
    return r == 0 ? 9 : r;
}

const int N = 10000;

int is_prime[N * 3];

void sieve() {
    int N3 = N * 3;
    for (int i = 3; i < N3; i += 2) {
        is_prime[i] = 1;
    }
    is_prime[2] = 1;
    for (int i = 3; i * i < N3; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N3; j += i * 2) {
                is_prime[j] = 0;
            }
        }
    }
}

int main() {
    sieve();

    vector<int> P, C;
    for (int n = 2; n < N * 3; n++) {
        if (is_prime[n]) {
            P.push_back(digital_root(n));
        }
        else {
            C.push_back(digital_root(n));
        }
    }
    P.resize(N);
    C.resize(N);

    for (int i = 0; i < 10; i++) {
        cout << P[i] << " ";
    }
    cout << "\n";
    for (int i = 0; i < 10; i++) {
        cout << C[i] << " ";
    }
    cout << "\n";
}