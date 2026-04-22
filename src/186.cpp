#include <iostream>
#include <assert.h>
#include <deque>

using namespace std;

int const N = 1e6;

struct LaggedFibRng {
    int k;
    deque<int> window;

    LaggedFibRng() : k(1) {
        for (int i = 1; i <= 55; i++) {
            long long val = (100003LL - 200003LL * i + 300007LL * i * i * i) % 1000000;
            if (val < 0) {
                val += 1000000;
            }
            window.push_back((int)val);
        }
        assert(window[0] == 200007);
        assert(window[1] == 100053);
    }

    int next() {
        if (k <= 55) {
            return window[k++ - 1];
        }
        int next_val = (window[31] + window[0]) % 1000000;
        window.pop_front();
        window.push_back(next_val);
        k++;
        return next_val;
    }
};

int par[N + 1];

void init_dsu() {
    for (int i = 0; i <= N; i++) {
        par[i] = -1;
    }
}

int root(int u) {
    return (par[u] < 0 ? u : par[u] = root(par[u]));
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

int const PM = 524287;
int const target_connected = (int)(N * 0.99);

int main() {
    LaggedFibRng rng;
    init_dsu();

    int misdials = 0;
    for (int i = 1; ; i++) {
        int u = rng.next(), v = rng.next();
        if (u == v) {
            misdials++;
            continue;
        }
        merge(u, v);
        if (-par[root(PM)] >= target_connected) {
            cout << i - misdials << '\n';
            break;
        }
    }
}