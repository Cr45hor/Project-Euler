#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

int const N = 5e7;

int cnt[N];

int main() {
    #pragma omp parallel for schedule(dynamic, 256)
    for (int x = 2; x < N; x++) {
        for (int d = (x / 4) + 1; d < x; d++) {
            int n = x * (4 * d - x);
            if (n >= N) {
                break;
            }
            #pragma omp atomic
            cnt[n]++;
        }
    }

    cout << count_if(cnt, cnt + N, [](int x) { return x == 1; }) << '\n';
}