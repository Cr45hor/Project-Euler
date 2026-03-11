#include <iostream>
#include <assert.h>

using namespace std;

int const N = 1000000;

int dr(int x) {
    assert(x > 1);
    return (x - 1) % 9 + 1;
}

int mdrs[N + 1];

int main() {
    mdrs[1] = 1;
    for (int x = 2; x <= N; x++) {
        mdrs[x] = max(mdrs[x], dr(x));
        
        int max_k = min(N / x, x);
        for (int k = 2; k <= max_k; k++) {
            mdrs[k * x] = max(mdrs[k * x], mdrs[x] + mdrs[k]);
        }
    }

    long long sum = 0;
    for (int x = 2; x < N; x++) {
        sum += mdrs[x];
    }
    cout << sum << '\n';
}