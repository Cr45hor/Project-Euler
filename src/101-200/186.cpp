#include <iostream>
#include <cassert>

using namespace std;

int const N = 1e6;

struct LaggedFibRng {
    static const int MOD = 1000000;
    static const int HALF = 55;

    int buf[HALF * 2];
    int active_start;
    int offset;

    void fill_half(int src_start, int dst_start) {
        for (int j = 0; j < HALF; j++) {
            int from_k_minus_55 = buf[src_start + j];
            int from_k_minus_24 = (j < 24 ? buf[src_start + j + 31] : buf[dst_start + j - 24]);
            buf[dst_start + j] = (from_k_minus_55 + from_k_minus_24) % MOD;
        }
    }

    LaggedFibRng() : active_start(0), offset(0) {
        for (int i = 1; i <= HALF; i++) {
            long long val = (100003LL - 200003LL * i + 300007LL * i * i * i) % MOD;
            if (val < 0) {
                val += MOD;
            }
            buf[i - 1] = (int)val;
        }
        assert(buf[0] == 200007);
        assert(buf[1] == 100053);

        fill_half(0, HALF);
    }

    int next() {
        int val = buf[active_start + offset];
        offset++;

        if (offset == HALF) {
            offset = 0;
            if (active_start == 0) {
                active_start = HALF;
                fill_half(HALF, 0);
            } else {
                active_start = 0;
                fill_half(0, HALF);
            }
        }

        return val;
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