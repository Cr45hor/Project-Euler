#include <iostream>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <map>
#include <set>

using namespace std;

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

struct PRNG_offset : public PRNG {
    int offset;

    PRNG_offset(int s0, int modulo, int offset) : PRNG(s0, modulo), offset(offset) {
        assert(!(offset & 1));
    }

    int normalize(int x) {
        assert(x >= 0);
        return (x % offset) - (offset / 2);
    }

    int curr() {
        return normalize(PRNG::curr());
    }

    int next() {
        return normalize(PRNG::next());
    }
};

struct Point {
    int x = 0;
    int y = 0;

    Point() {}
    Point(int __x, int __y): x(__x), y(__y) {}

    Point operator-(const Point &other) const {
        return Point(x - other.x, y - other.y);
    }

    pair<int, int> normalize() const {
        int g = gcd(abs(x), abs(y));
        int sgn = 1;
        if (x < 0 || (x == 0 && y < 0)) {
            sgn = -1;
        }
        return {sgn * x / g, sgn * y / g};
    }

    friend ostream& operator<<(ostream &os, const Point &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

int const N = 2500;

Point T[N];
map<pair<int, int>, int> dir;

int main() {
    PRNG_offset prng(seed, MOD, 2000);
    prng.next();

    for (int i = 0; i < N; i++) {
        T[i] = Point{prng.curr(), prng.next()};
        prng.next();
    }

    set<tuple<int, int, int>> unique_lines;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            Point v = T[j] - T[i];
            auto [a, b] = T[i];
            auto [m, n] = v.normalize();
            tuple<int, int, int> t = {m, n, a * n - b * m};
            if (unique_lines.find(t) == unique_lines.end()) {
                unique_lines.insert(t);
                dir[{m, n}]++;
            }
        }
    }

    int M = (int)unique_lines.size();
    int64_t tot_pairs = (int64_t)M * (M - 1);
    int64_t res = tot_pairs;
    for (auto &[d, cnt]: dir) {
        res -= (int64_t)cnt * (cnt - 1);
    }
    cout << res << '\n';
}