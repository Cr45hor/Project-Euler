#include <iostream>
#include <vector>

using namespace std;

long long count_mixed_pairs(int n) {
    if (n < 4) return 0;

    vector<long long> T(n + 1, 0);
    T[0] = 1;
    T[1] = 1;
    for (int i = 2; i <= n; i++) {
        T[i] = ((2 * i - 1) * T[i - 1] + 3 * (i - 1) * T[i - 2]) / i;
    }

    // Motzkin
    vector<long long> M(n + 1, 0);
    M[0] = 1;
    M[1] = 1;
    for (int i = 2; i <= n; i++) {
        M[i] = ((2 * i + 1) * M[i - 1] + 3 * (i - 1) * M[i - 2]) / (i + 2);
    }

    return (T[n] - 2 * M[n] + 1) / 2;
}

int main() {
    int n = 12;
    long long ans = count_mixed_pairs(n);
    cout << ans << '\n';

    return 0;
}