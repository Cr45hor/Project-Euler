#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdint>

using namespace std;

int main() {
    long double const log10_2 = log10l(2.0L);

    int64_t n = 6;
    int cnt = 0;
    while(cnt < 678910) {
        n++;
        int64_t len = (int64_t)ceil(log10_2 * n);
        int64_t offset = len - 3;
        int prefix = (int)floor(powl(10.0L, log10_2 * n - offset));
        if (prefix == 123) {
            cnt++;
            // cout << cnt << ' ' << n << ' ' << prefix << ' ' << len << '\n';
        }
    }
    cout << n << '\n';
}