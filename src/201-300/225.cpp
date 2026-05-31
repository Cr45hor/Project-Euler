#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <tuple>
#include <vector>

using namespace std;

int const MAX_RES = 124;

int res[MAX_RES];
int cnt_res = 0;

int mapping(int base, int a, int b, int c) {
    return a * base * base + b * base + c;
}

bool check(int m) {
    int T[4] = {1, 1, 1, 0};
    unordered_set<int> s;
    s.insert(mapping(m, T[0], T[1], T[2]));
    for (int i = 3; ; i++) {
        T[i % 4] = (T[(i - 1) % 4] + T[(i - 2) % 4] + T[(i - 3) % 4]) % m;
        if (T[i % 4] == 0) {
            return false;
        }
        int next_t = mapping(m, T[i % 4], T[(i - 1) % 4], T[(i - 2) % 4]);
        if (s.count(next_t)) {
            return true;
        }
        s.insert(next_t);
    }
}

int main() {
    for (int m = 27; cnt_res < MAX_RES; m += 2) {
        if (check(m)) {
            res[cnt_res++] = m;
        }
    }
    cout << res[MAX_RES - 1] << '\n';
}