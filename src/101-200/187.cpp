#include <iostream>

using namespace std;

int const N = 1e8;

int spf[N];

int main() {
    spf[1] = 1;
    for (int x = 2; x < N; x++) {
        if (spf[x])
            continue;
        spf[x] = x;
        if (1LL * x * x > N) 
            continue;
        for (int y = x * x; y < N; y += x) {
            if (!spf[y]) {
                spf[y] = x;
            }
        }
    }

    int res = 0;
    for (int x = 4; x < N; x++) {
        if (spf[x] == x) 
            continue;
        int y = x / spf[x];
        if (spf[y] == y) {
            res++;
        }
    }
    cout << res << endl;
}