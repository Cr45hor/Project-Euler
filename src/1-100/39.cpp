#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;

int num_solutions(int p) {
    assert(!(p & 1)); // p must be even

    int cnt = 0;
    for (int c = 1; c <= p; c++) {
        int A = p - c;
        int B = p * (p - 2 * c) / 2;

        int delta = A * A - 4 * B;
        if (delta < 0) {
            continue;
        }
        int sqrt_delta = (int)sqrt(delta);
        if (sqrt_delta * sqrt_delta != delta) {
            continue;
        }

        if ((A & 1) != (sqrt_delta & 1)) {
            continue;
        }
        if (A < sqrt_delta) {
            continue;
        }
        int a = (A - sqrt_delta) / 2, b = A - a;
        if (a < 0 || b < 0) {
            continue;
        }
        cnt += (b < c);
    }
    return cnt;
}

int main() {
    pair<int, int> best = {0, 0};
    for (int p = 12; p <= 1000; p += 2) {
        best = max(best, {num_solutions(p), p});
    }
    cout << best.second << endl;
    return 0;
}