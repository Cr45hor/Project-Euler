#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cstdint>
#include <set>
#include <map>

using namespace std;

const int N = 5;

int par[1 << N];

void reset() {
    for (int i = 0; i < (1 << N); i++) {
        par[i] = -1;
    }
}

int root(int u) {
    return (par[u] < 0 ? u : par[u] = root(par[u]));
}

int size(int u) {
    return -par[root(u)];
}

bool merge(int u, int v) {
    u = root(u);
    v = root(v);
    if (u == v) {
        return false;
    }

    if (par[u] > par[v]) {
        swap(u, v);
    }
    par[u] += par[v];
    par[v] = u;
    return true;
}

map<int, int> f(int n) {
    int nn = 1 << n;
    map<int, int> freq;

    for (long long mask = 1; mask < (1LL << nn); mask++) {
        if (mask & 1) {
            continue;
        }
        vector<int> bits;
        for (int i = 0; i < nn; i++) {
            if (mask & (1LL << i)) {
                bits.push_back(i);
            }
        }

        reset();
        for (int i = 0; i < (int)bits.size(); i++) {
            for (int j = i + 1; j < (int)bits.size(); j++) {
                if (bits[i] & bits[j]) {
                    merge(bits[i], bits[j]);
                }
            }
        }

        set<int> roots;
        for (int i = 0; i < (int)bits.size(); i++) {
            roots.insert(root(bits[i]));
        }
        freq[(int)roots.size()]++;
    }
    return freq;
}

int main() {
    for (int n = 1; n <= 4; n++) {
        cout << "n = " << n << '\n';
        auto freq = f(n);
        for (auto [k, v] : freq) {
            cout << k << ": " << v << '\n';
        }
    }
}