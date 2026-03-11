#include <iostream>
#include <math.h>
#include <assert.h>
#include <numeric>

using namespace std;

int const N = 100;
long double const e = 2.71828182845904523536028747135266249775724709369995L;

int k(int n) {
    // Return the value k that maximizes (n/k)^k
    return (int)round(n / e);
}

int D(int n) {
    int k_n = k(n);
    int g = gcd(n, k_n);
    k_n /= g;

    for (int d: {2, 5}) {
        while (k_n % d == 0) {
            k_n /= d;
        }
    }

    return (k_n == 1 ? -n : n);
}

int main() {
    long long sum = 0;
    for (int n = 5; n <= N; n++) {
        sum += D(n);
    }

    cout << sum;
}