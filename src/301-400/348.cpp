#include <iostream>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <cassert>

using namespace std;

struct PairHash {
    size_t operator()(const pair<int, int>& p) const noexcept {
        return (static_cast<size_t>(p.first) << 32) ^ static_cast<size_t>(p.second);
    }
};

bool is_palindrone(int64_t x) {
    int64_t y = 0, z = x;
    while (z > 0) {
        y = y * 10 + z % 10;
        z /= 10;
    }
    return x == y;
}

struct State {
    int x = 0;
    int y = 0;
    int64_t val = 0;

    State(int __x, int __y) : x(__x), y(__y) {
        val = 1LL * x * x * x + 1LL * y * y;
    }

    bool operator<(const State &other) const {
        if (val != other.val) {
            return val > other.val; // Min-heap based on val
        }
        // assert(x != other.x);
        return x > other.x;
    }
};

int main() {
    priority_queue<State> Q;
    unordered_map<pair<int, int>, bool, PairHash> visited;
    Q.push(State(1, 1));

    int cnt = 0;
    int64_t sum = 0;

    int64_t lst = 0;
    int64_t freq = 0;
    int max_x = 0;
    int max_y = 0;
    int tmp_x = 0;
    int tmp_y = 0;

    while(cnt < 5) {
        State cur = Q.top();
        Q.pop();

        auto [x, y, val] = cur;
        if (visited.find({x, y}) != visited.end()) {
            continue;
        }
        visited[{x, y}] = true;
        if (val == lst) {
            freq++;
            tmp_x = max(tmp_x, x);
            tmp_y = max(tmp_y, y);
        }
        else {
            if (freq == 4 && is_palindrone(lst)) {
                cnt++;
                sum += lst;
                max_x = max(max_x, tmp_x);
                max_y = max(max_y, tmp_y);
                // cout << lst << '\n';
            }
            lst = val;
            freq = 1;
            tmp_x = x;
            tmp_y = y;
        }

        Q.push(State(x + 1, y));
        Q.push(State(x, y + 1));
    }

    cout << max_x << ' ' << max_y << '\n';
    cout << sum << '\n';
}