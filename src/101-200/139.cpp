#include <iostream>
#include <assert.h>
#include <numeric>

using namespace std;

int const N = 1e8;
int const N_sqrt = 1e4;

int main() {
    long long cnt = 0;

    for (int n = 1; n <= N_sqrt; n++) {
        for (int m = n + 1; m <= N_sqrt - n; m += 2) {
            long long a = m * m - n * n;
            long long b = 2 * m * n;
            long long c = m * m + n * n;
            long long sum = a + b + c;

            if (gcd(m, n) == 1 && sum < N && c % abs(b - a) == 0) {
                cnt += N / sum;
            }
        }
    }

    cout << cnt;
}