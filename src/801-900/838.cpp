#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int const N = 1e6;

int spf[N];

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
}

int binpow(int a, int n, int mod) {
    int res = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = (int64_t)res * a % mod;
        }
        a = (int64_t)a * a % mod;
    }
    return res;
}

int buf[8];

int factor(int n) {
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        while (n % p == 0) {
            n /= p;
        }
        buf[cnt++] = p;
    }
    return cnt;
}

int mandatory[N];

set<pair<int, int>> prime_rank;
vector<int> multiples[N];
vector<int> factors[N];

void update(int n) {
    if (spf[n] == n) {
        mandatory[n]++;
        return;
    }

    int l = factor(n);
    if (all_of(buf, buf + l, [&](int p) { return mandatory[p] == 0; })) {
        factors[n] = vector<int>(buf, buf + l);
        for (int p: factors[n]) {
            multiples[p].push_back(n);
        }
    }
}

int main() {
    sieve();

    for (int n = 3; n <= N; n += 10) {
        update(n);
    }

    for (int p = 2; p < N; p++) {
        if (spf[p] == p && multiples[p].size() > 0) {
            prime_rank.insert({-(int)multiples[p].size(), p});
        } 
    }

    while((int)prime_rank.size() > 0) {
        auto [neg_rank, smallest_p] = *prime_rank.begin();
        prime_rank.erase(prime_rank.begin());
        mandatory[smallest_p]++;

        for (int n: multiples[smallest_p]) {
            for (int p: factors[n]) if (p != smallest_p) {
                prime_rank.erase({-(int)multiples[p].size(), p});
                multiples[p].erase(remove(multiples[p].begin(), multiples[p].end(), n), multiples[p].end());
                if (multiples[p].size() > 0) {
                    prime_rank.insert({-(int)multiples[p].size(), p});
                }
            }
        }
    }

    double res = 0;
    for (int p = 2; p < N; p++) {
        if (spf[p] == p && mandatory[p] > 0) {
            res += log(p);
        }
    }

    cout << setprecision(6) << fixed << res << '\n';
}