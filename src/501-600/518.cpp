#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <random>
#include <chrono>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int const N = 1e8;

int spf[N];
uint64_t h[N];
vector<int> primes;

void sieve() {
    for (int x = 1; x < N; x++) {
        spf[x] = x;
    }
    for (int x = 4; x < N; x += 2) {
        spf[x] = 2;
    }
    for (int x = 3; x * x < N; x += 2) {
        if (spf[x] == x) {
            for (int y = x * x; y < N; y += x * 2) {
                if (spf[y] == y) {
                    spf[y] = x;
                }
            }
        }
    }

    for (int p = 2; p < N; p++) {
        if (spf[p] == p) {
            primes.push_back(p);
            h[p] = uniform_int_distribution<uint64_t>(numeric_limits<uint64_t>::min(), numeric_limits<uint64_t>::max())(rng);
        }
    }
}

bool exist(int n) {
    return spf[n - 1] == n - 1;
}

array<int, 16> factor(int n) {
    int cnt = 0;
    array<int, 16> buf{};
    while (n > 1) {
        int p = spf[n];
        int k = 0;
        while (n % p == 0) {
            n /= p;
            k++;
        }
        buf[cnt << 1] = p;
        buf[(cnt << 1) | 1] = k;
        cnt++;
    }
    return buf;
}

uint64_t H(int n) {
    array<int, 16> buf = factor(n);
    uint64_t res = 0;
    for (int i = 0; i < 16; i += 2) {
        if (buf[i] == 0) break;
        if (buf[i + 1] & 1) {
            res ^= h[buf[i]];
        }
    }
    return res;
}

int main() {
    sieve();

    vector<pair<uint64_t, int>> L;
    for (int p: primes) {
        L.emplace_back(H(p + 1), p + 1);
    }

    sort(L.begin(), L.end());

    int64_t S = 0;
    int n = (int)L.size();
    for (int l = 0, r = 0; l < n; l = r) {
        while(r < n && L[l].first == L[r].first) {
            r++;
        }
        if (r - l > 1) {
            for (int i = l; i < r; i++) {
                for (int j = i + 1; j < r; j++) {
                    int a = L[i].second;
                    int c = L[j].second;
                    int b = (int)sqrt((int64_t)a * c);
                    if (exist(b)) {
                        S += a + b + c - 3;
                    }
                }
            }
        }
    }
    cout << S << '\n';
}