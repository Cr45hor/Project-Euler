#include <iostream>
#include <cstdint>
#include <cassert>
#include <numeric>

#include "../Z.hpp"
const int MOD = 1e9 + 7;

using namespace std;

const int N = 1e7;

int spf[N];
int phi[N];

void sieve() {
    for (int i = 1; i < N; i++) {
        spf[i] = i;
    }
    for (int i = 2; i < N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    phi[1] = 1;
    for (int n = 2; n < N; n++) {
        int p = spf[n];
        int m = n / p;
        if (m % p == 0) {
            phi[n] = phi[m] * p;
        }
        else {
            phi[n] = phi[m] * (p - 1);
        }
    }
}

int pow_v3(int n) {
    int res = 1;
    while(n % 3 == 0) {
        n /= 3;
        res *= 3;
    }
    return res;
}

int buf[16];
int factorize(int n) {
    int l = 0;
    while(n > 1) {
        int p = spf[n];
        buf[l++] = p;
        while(n % p == 0) {
            n /= p;
        }
    }
    return l;
}

pair<int, int> A(int n) {
    int p = pow_v3(n);
    int q = phi[n / p];
    int l = factorize(q);
    for (int i = 0; i < l; i++) {
        while(q % buf[i] == 0 && binpow(10, q / buf[i], n) == 1) {
            q /= buf[i];
        }
    }
    return {p, q};
}

const int THRESHOLD = 1e6;

int main() {
    sieve();
    
    int cnt = 0;
    int64_t sum = 0;
    for (int n = 3; n <= THRESHOLD; n++) {
        if (gcd(n, 10) != 1) {
            continue;
        }
        auto [p, q] = A(n);
        if ((n - 1) % lcm<int64_t>(p, q) == 0 && spf[n] < n) {
            cnt++;
            sum += n;
            cout << n << ' ' << p << ' ' << q << '\n';
            if (cnt == 25) {
                cout << "TOTAL: " << sum << '\n';
                break;
            }
        }
    }
}