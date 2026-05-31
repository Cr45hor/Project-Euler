#include <iostream>
#include <assert.h>
#include <math.h>
#include <numeric>
#include <algorithm>

using namespace std;

int const N = 15e5;

int cnt[N + 1];

int main() {
    // Generate all primitive Pythagorean triples with perimeter <= N
    int max_n = (int)sqrt(N)/2;
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
            for (int k = 1; k * p <= N; k++) {
                cnt[k * p]++;
            }
        }
    }

    cout << count_if(cnt, cnt + N + 1, [](int x) { return x == 1; }) << '\n';
    return 0;
}