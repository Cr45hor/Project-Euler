#include <iostream>
#include <numeric>
#include <cassert>

using namespace std;

int const N = 1e8;

int spf[N + 1];

void sieve() {
    for (int n = 1; n <= N; n++) {
        spf[n] = n;
    }

    for (int p = 2; p * p <= N; p++) {
        if (spf[p] == p) {
            for (int j = p * p; j <= N; j += p) {
                if (spf[j] == j) {
                    spf[j] = p;
                }
            }
        }
    }
}

int v(int p, int n) {
    int ans = 0;
    while (n % p == 0) {
        n /= p;
        ans++;
    }
    return ans;
}

int s[N + 1];

int main() {
    sieve();

    s[1] = 0;
    for (int n = 2; n <= N; n++) {
        if (s[n] > 0) {
            continue;
        }

        if (spf[n] == n) {
            int p = n;
            int p_multiple = 0;
            int sum_v = 0;

            int pk = 1;
            int k = 0;

            while(1LL * pk * p <= N) {
                pk *= p;
                k++;

                while (sum_v < k) {
                    p_multiple += p;
                    sum_v += v(p, p_multiple);
                }

                s[pk] = p_multiple;
            }
            continue;
        }
        
        int p = spf[n];
        int m = n;
        while(m % p == 0) {
            m /= p;
        }
        s[n] = max(s[n / m], s[m]);
    }

    cout << accumulate(s + 2, s + N + 1, 0LL) << '\n';
    return 0;
}