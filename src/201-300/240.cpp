#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

using namespace std;

int const S = 70;
int const side = 12;
int const N = 10;

int64_t P[N * side + 1];
int64_t tmp[N * side + 1];
int64_t PN[N * side + 1];

int main() {
    for (int i = 1; i <= side; i++) {
        P[i] = 1;
        PN[i] = 1;
    }

    for (int k = 2; k <= N; k++) {
        for (int i = 0; i <= side * N; i++) {
            tmp[i] = 0;
        }
        for (int i = 0; i <= side * N; i++) {
            for (int j = 1; j <= side; j++) {
                if (i + j <= side * N) {
                    tmp[i + j] += PN[i];
                }
            }
        }
        for (int i = 0; i <= side * N; i++) {
            PN[i] = tmp[i];
        }

        cout << k << ":\n";
        for (int i = 1; i <= side * N; i++) {
            cout << "P(" << i << ", " << k << ") = " << PN[i] << "\n";
        }
    }

    cout << PN[S] << '\n';
}