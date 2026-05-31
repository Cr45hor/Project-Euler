#include <iostream>
#include <assert.h>
#include <math.h>
#include <numeric>
#include <algorithm>

using namespace std;

int const N = 1e9;

int main() {
    int max_n = (int)sqrt(N)/2;
    long long sum = 0;
    for (int n = 1; n <= max_n; n++) {
        for (int m = n + 1; ; m++) {
            if (gcd(m, n) != 1 || (m & 1) == (n & 1)) {
                continue;
            }
            int a = m * m - n * n;
            int b = 2 * m * n;
            int c = m * m + n * n;
            int p = a + b + c;
            if (p > N) {
                break;
            }
            assert (!(abs(c - a * 2) == 1 && abs(c - b * 2) == 1));
            if (abs(c - b * 2) == 1) {
                swap(a, b);
            } 
            if (abs(c - a * 2) == 1) {
                sum += 2LL * (a + c);
            }
        }
    }
    cout << sum << '\n';

    return 0;
}