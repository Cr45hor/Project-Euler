#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

const int CLOCK = 12;
const int MOD = 50515093;
const int64_t seed = 290797;

struct PRNG {
    int s;
    int m;

    PRNG(int s0, int modulo) : s(s0), m(modulo) {}
    
    int curr() {
        return s;
    }

    int next() {
        s = (int64_t(s) * s) % m;
        return s;
    }
};

struct Node {
    int offset = 0;
    int cnt[CLOCK]{};

    Node(int len = 0): offset(0) {
        for (int v = 0; v < CLOCK; v++) {
            cnt[v] = 0;
        }
        cnt[CLOCK - 1] = len;
    }

    void add_offset(int delta) {
        delta %= CLOCK;
        if (delta < 0) {
            delta += CLOCK;
        }

        int cnt_tmp[CLOCK]{};
        for (int v = 0; v < CLOCK; v++) {
            cnt_tmp[(v + delta) % CLOCK] = cnt[v];
        }
        for (int v = 0; v < CLOCK; v++) {
            cnt[v] = cnt_tmp[v];
        }
        offset = (offset + delta) % CLOCK;
    }

    int sum() const {
        int res = cnt[0] * CLOCK; // index 0 maps to 12
        for (int v = 1; v < CLOCK; v++) {
            res += cnt[v] * v;
        }
        return res;
    }
};

void push(Node &par, Node &ls, Node &rs) {
    if (par.offset > 0) {
        ls.add_offset(par.offset);
        rs.add_offset(par.offset);
        par.offset = 0;
    }
}

void combine(Node &par, Node &ls, Node &rs) {
    for (int v = 0; v < CLOCK; v++) {
        par.cnt[v] = ls.cnt[v] + rs.cnt[v];
    }
}

Node ST[MOD << 2];

void init(int id, int l, int r) {
    if (l == r) {
        ST[id] = Node(1);
        return;
    }
    int mid = (l + r) >> 1;
    init(id << 1, l, mid);
    init(id << 1 | 1, mid + 1, r);
    combine(ST[id], ST[id << 1], ST[id << 1 | 1]);
}

void init() {
    init(1, 0, MOD - 1);
}

void update(int id, int l, int r, int ql, int qr, int delta) {
    if (qr < l || r < ql) {
        return;
    }
    if (ql <= l && r <= qr) {
        ST[id].add_offset(delta);
        return;
    }
    int mid = (l + r) >> 1;
    push(ST[id], ST[id << 1], ST[id << 1 | 1]);
    update(id << 1, l, mid, ql, qr, delta);
    update(id << 1 | 1, mid + 1, r, ql, qr, delta);
    combine(ST[id], ST[id << 1], ST[id << 1 | 1]);
}

void update(int l, int r, int delta) {
    update(1, 0, MOD - 1, l, r, delta);
}

int query() {
    return ST[1].sum();
}

const int N = 1e5;

int rect[N + 1][4]{};

int main() {
    PRNG prng(seed, MOD);
    
    rect[0][0] = rect[0][2] = 0;
    rect[0][1] = rect[0][3] = MOD - 1;
    for (int t = 1; t <= N; t++) {
        for (int i = 0; i < 4; i++) {
            rect[t][i] = prng.curr();
            prng.next();
        }
    }

    vector<tuple<int, int, int, int>> events;
    for (int t = 0; t <= N; t++) {
        if (rect[t][0] > rect[t][1]) {
            swap(rect[t][0], rect[t][1]);
        }
        if (rect[t][2] > rect[t][3]) {
            swap(rect[t][2], rect[t][3]);
        }

        events.emplace_back(rect[t][2], rect[t][0], rect[t][1], 1);
        events.emplace_back(rect[t][3] + 1, rect[t][0], rect[t][1], -1);
    }
    sort(events.begin(), events.end());

    init();
    int lst_y = 0;
    int64_t ans = 0;
    for (int i = 0, j = 0; i < (int)events.size(); i = j) {
        int y = get<0>(events[i]);
        ans += 1LL * query() * (y - lst_y);
        lst_y = y;

        while(j < (int)events.size()) {
            auto [y_next, l, r, delta] = events[j];
            if (y_next != y) {
                break;
            }
            update(l, r, delta);
            j++;
        }
    }
    cout << ans << '\n';
}