#include <iostream>
#include <cassert>
#include <cstdint>

#include "../Z.hpp"
int const MOD = 1e9;

using namespace std;

int const N = 10000;
Z moves[N + 1];

Z step(Z n, Z a, Z b, int dir) {
    if (dir == 0) {
        // a -> b
        return (b - a) * (a + b - 2); 
    }
    else {
        // b -> a
        return (a - b) * (n * 2 - a - b);
    }
}

Z m(int n, int a, int b) {
    int offset = 0;
    if ((n & 1) && a == 0 && b == 2) {
        offset = 1;
    }
    else if (!(n & 1) && a == 2 && b == 0) {
        offset = -1;
    }
    return moves[n - 1] + offset;
}

Z E(int n, Z len, Z pos[3]) {
    Z res = step(len, pos[1], pos[0], 1);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i != j) {
                res += m(n, i, j) * step(len, pos[i], pos[j], (i > j));
            }
        }
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    
    for (int n = 1; n <= N; n++) {
        moves[n] = (moves[n - 1] * 2) + (n & 1);
    }

    Z coef[4] = {10, 3, 6, 9};
    Z parameter[4] = {Z(1), Z(1), Z(1), Z(1)};
    Z res = 0;
    for (int n = 1; n <= N; n++) {
        for (int i = 0; i < 4; i++) {
            parameter[i] *= coef[i];
        }
        res += E(n, parameter[0], parameter + 1);
    }
    cout << res << '\n';
}