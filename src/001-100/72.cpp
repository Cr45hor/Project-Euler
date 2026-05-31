#include <iostream>
#include <numeric>

using namespace std;

int const N = 1e6;

int phi[N + 1];

int main() {
    for (int i = 1; i <= N; i++) {
        phi[i] = i;
    }

    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= N; j += i) {
                phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }

    cout << accumulate(phi + 1, phi + N + 1, -1LL) << endl;
}