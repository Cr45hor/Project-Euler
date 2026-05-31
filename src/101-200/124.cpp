#include <iostream>
#include <algorithm>

using namespace std;

int const N = 1e5;

int spf[N + 1];

void sieve() {
    for (int i = 1; i <= N; i++) {
        spf[i] = i;
    }
    for (int i = 2; i * i <= N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int rad[N + 1];
pair<int, int> a[N + 1];

int main() {
    sieve();
    
    rad[1] = 1;
    for (int n = 2; n <= N; n++) {
        int p = spf[n];
        int q = n;
        while(q % p == 0) {
            q /= p;
        }
        rad[n] = p * rad[q];
    }

    for (int i = 1; i <= N; i++) {
        a[i] = {rad[i], i};
    }
    sort(a + 1, a + N + 1);
    cout << a[10000].second << '\n';
}