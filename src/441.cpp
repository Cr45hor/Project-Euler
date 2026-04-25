#include <iostream>
#include <iomanip>

using namespace std;

int const N = 1e7;

double harmonic_sum = 0.0;
double F[N + 1];

int main() {
    for (int i = 1; i <= N; i++) {
        F[i] = harmonic_sum * 2 / i;
        harmonic_sum += 1.0 / i;
    }

    for (int p = 1; p <= N; p++) {
        for (int q = 2; p * q <= N; q++) {
            F[p * q] -= F[p] / q / q;
        }
    }

    double res = 0;
    for (int i = 2; i <= N; i++) {
        res += 0.5 + F[i] / 2.0;
    }
    cout << setprecision(4) << fixed << res - 0.5 << '\n';
}