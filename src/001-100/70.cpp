#include <iostream>
#include <numeric>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;

int const N = 1e7;

int phi[N];

bool check_permutation(int a, int b) {
    assert(a > 0 && a < N);
    assert(b > 0 && b < N);

    string sa = to_string(a);
    string sb = to_string(b);

    sort(sa.begin(), sa.end());
    sort(sb.begin(), sb.end());

    return sa == sb;
}

int main() {
    for (int i = 1; i < N; i++) {
        phi[i] = i;
    }

    for (int i = 2; i < N; i++) {
        if (phi[i] == i) {
            for (int j = i; j < N; j += i) {
                phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }

    pair<double, int> ans = {2, 2};
    for (int x = 3; x < N; x++) {
        if (check_permutation(x, phi[x])) {
            double ratio = (double)x / phi[x];
            ans = min(ans, {ratio, x});
        }
    }
    cout << ans.second << '\n';
    return 0;
}