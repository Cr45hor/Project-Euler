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

int normalize(int x) {
    if (x < 0) {
        x += CLOCK;
    }
    else if (x >= CLOCK) {
        x -= CLOCK;
    }
    return x;
}

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
        delta = normalize(delta);

        int cnt_tmp[CLOCK]{};
        for (int v = 0; v < CLOCK; v++) {
            cnt_tmp[normalize(v + delta)] = cnt[v];
        }
        for (int v = 0; v < CLOCK; v++) {
            cnt[v] = cnt_tmp[v];
        }
        offset = normalize(offset + delta);
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

// Maximum distinct X coordinates: (N + 1 rectangles) * 2 endpoints
const int MAX_X = 200005; 
Node ST[MAX_X << 2];
vector<int> xs; // Global vector to store our compressed coordinates

void init(int id, int l, int r) {
    if (l == r) {
        // The length of this specific interval
        ST[id] = Node(xs[l + 1] - xs[l]); 
        return;
    }
    int mid = (l + r) >> 1;
    init(id << 1, l, mid);
    init(id << 1 | 1, mid + 1, r);
    combine(ST[id], ST[id << 1], ST[id << 1 | 1]);
}

// update() functions stay exactly the same logic-wise, 
// but 'l' and 'r' now represent indices of the 'xs' array.
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

int query() {
    return ST[1].sum();
}

struct Event {
    int y, l_idx, r_idx, delta;
    bool operator<(const Event& o) const {
        return y < o.y;
    }
};

const int N = 1e5;
int rect[N + 1][4]{};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    PRNG prng(seed, MOD);
    
    rect[0][0] = rect[0][2] = 0;
    rect[0][1] = rect[0][3] = MOD - 1;
    for (int t = 1; t <= N; t++) {
        for (int i = 0; i < 4; i++) {
            rect[t][i] = prng.curr();
            prng.next();
        }
    }

    // 1. Collect coordinates
    for (int t = 0; t <= N; t++) {
        if (rect[t][0] > rect[t][1]) swap(rect[t][0], rect[t][1]);
        if (rect[t][2] > rect[t][3]) swap(rect[t][2], rect[t][3]);
        
        xs.push_back(rect[t][0]);
        xs.push_back(rect[t][1] + 1);
    }
    
    // 2. Sort and unique
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    
    int M = xs.size() - 1; 
    init(1, 0, M - 1);

    // 3. Build events using pre-calculated compressed indices
    vector<Event> events;
    events.reserve((N + 1) * 2); // Prevent memory reallocation

    for (int t = 0; t <= N; t++) {
        int l_idx = lower_bound(xs.begin(), xs.end(), rect[t][0]) - xs.begin();
        int r_idx = lower_bound(xs.begin(), xs.end(), rect[t][1] + 1) - xs.begin() - 1;
        
        if (l_idx <= r_idx) {
            events.push_back({rect[t][2], l_idx, r_idx, 1});
            events.push_back({rect[t][3] + 1, l_idx, r_idx, -1});
        }
    }
    
    sort(events.begin(), events.end());

    // 4. Ultra-fast sweep line loop
    int lst_y = 0;
    int64_t ans = 0;
    
    for (int i = 0, j = 0; i < (int)events.size(); i = j) {
        int y = events[i].y;
        ans += 1LL * query() * (y - lst_y);
        lst_y = y;

        while(j < (int)events.size() && events[j].y == y) {
            update(1, 0, M - 1, events[j].l_idx, events[j].r_idx, events[j].delta);
            j++;
        }
    }
    cout << ans << '\n';
    return 0;
}