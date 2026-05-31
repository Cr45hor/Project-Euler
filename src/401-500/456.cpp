#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <map>

using namespace std;

struct RNG_gen {
    // A simple RNG generator
    // In form x_n = (s^n mod M) + offset
    // In this problem, M is an odd number and offset = (1 - M) / 2
    // Which mean each generated number is in the range [offset, offset + M - 1] = [(1 - M) / 2, (M - 1) / 2] (perfect symmetry around 0)
    // It is guaranteed that M < INT_MAX, so all computations can be done in 32-bit integers without overflow

    int seed = 1;
    int M = 1;
    int offset = 0;
    int current_index = 0;

    RNG_gen(int _seed, int _M): seed(_seed), M(_M) {
        offset = (1 - M) / 2;
    }

    // Since each number is generated one by one, no need to story history of generated numbers. Just keep track of the current state.
    int prod = 1;
    void update() {
        prod = (1LL * prod * seed) % M;
    }

    int next() {
        current_index++;
        update();
        return prod + offset;
    }
};

struct Point {
    int x = 0, y = 0;
    Point() {}
    Point(int __x, int __y): x(__x), y(__y) {}

    Point operator-() const { return Point(-x, -y); }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator<(const Point& other) const {
        // Used for map keys. Sort by x first, then by y.
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    int quad() const {
        assert(!(x == 0 && y == 0)); // Origin should not be compared
        if (x > 0 && y >= 0) return 1;
        if (x <= 0 && y > 0) return 2;
        if (x < 0 && y <= 0) return 3;
        if (x >= 0 && y < 0) return 4;
        assert(false);
    }

    Point reduced() const {
        int g = gcd(abs(x), abs(y));
        return Point(x / g, y / g);
    }
};

int32_t ccw(const Point& a, const Point& b) {
    // Purely for compute the counter-clockwise order of two points
    int64_t cross = 1LL * a.x * b.y - 1LL * a.y * b.x;
    if (cross > 0) return 1;
    if (cross < 0) return -1;
    return 0;
}

bool cmp_ccw(const Point& a, const Point& b) {
    // Used for sorting by angle
    if (a.quad() != b.quad()) return a.quad() < b.quad();
    return ccw(a, b) > 0;
}

// Bundle the Point with its count to eliminate the need for std::map

struct Ray {
    Point p;
    int count;
};

int const N = 2e6;

map<Point, int> reduced_cnt;

int main() {
    // // Fast I/O
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    // 1. Generated points
    RNG_gen X(1248, 32323);
    RNG_gen Y(8421, 30103);
    vector<Point> pts;

    for (int i = 1; i <= N; i++) {
        int x = X.next();
        int y = Y.next();
        Point P(x, y);
        reduced_cnt[P.reduced()]++;
        if (reduced_cnt[P.reduced()] == 1) {
            // Only store the first occurrence of each reduced direction in pts
            pts.push_back(P.reduced());
        }
    }

    // Sort by exact integer cross-product
    sort(pts.begin(), pts.end(), cmp_ccw);

    // 2. Bundle points with their counts
    vector<Ray> angles;
    angles.reserve(pts.size()); // Pre-allocate memory for speed
    for (const Point& p : pts) {
        angles.push_back({p, reduced_cnt[p]});
    }

    int tot_angles = angles.size();
    // Duplicate all angles for circular wrap-around
    for (int i = 0; i < tot_angles; i++) {
        angles.push_back(angles[i]);
        assert(angles[i].p == angles[i + tot_angles].p);
    }

    // 3. Prefix Sum Arrays
    int total_size = tot_angles * 2;
    vector<int> sum_pts(total_size, 0);
    vector<vector<int64_t>> sum_prod(2, vector<int64_t>(total_size, 0));

    sum_pts[0] = angles[0].count;
    for (int i = 1; i < total_size; i++) {
        sum_pts[i] = sum_pts[i - 1] + angles[i].count;
    }

    for (int type = 0; type < 2; type++) {
        sum_prod[type][0] = 1LL * sum_pts[0] * angles[type].count;
        for (int i = 1; i + type < total_size; i++) {
            sum_prod[type][i] = sum_prod[type][i - 1] + 1LL * sum_pts[i] * angles[i + type].count;
        }
    }

    // 4. Counting Logic
    // Replacing std::function with auto lambdas for inline optimization
    auto cnt_lattices = [&](int l, int r) -> int {
        if (l > r) return 0;
        return sum_pts[r] - (l == 0 ? 0 : sum_pts[l - 1]);
    };

    // 5. Precompute the opposite points for each angle to enable O(1) half-plane queries
    auto find_half = [&](int c_st, int c_end) -> pair<int, int> {
        assert(c_st < tot_angles);
        assert(c_end == c_st + tot_angles);

        // Range: [c_st, c_end)
        // If found_opposite is true, return the index of the opposite point
        // Else, return the last index that is strictly less than the opposite point (which is the boundary of the half-plane)

        int L = c_st, R = c_end;
        Point opposite = -angles[c_st].p;
        bool found_opposite = false;
        while(R - L > 1) {
            int mid = (L + R) / 2;
            if (angles[mid].p == opposite) {
                L = R = mid;
                found_opposite = true;
                break;
            }

            if (ccw(angles[mid].p, opposite) > 0) {
                L = mid;
            }
            else {
                R = mid;
            }
        }

        return make_pair(found_opposite, L);
    };

    vector<pair<int, int>> halfplane(tot_angles);
    vector<pair<int, int>> opposite_index;

    for (int i = 0; i < tot_angles; i++) {
        assert(angles[i].p == angles[i + tot_angles].p);
        auto [found, bound_index] = find_half(i, i + tot_angles);
        int L = bound_index - found, R = bound_index + 1;
        halfplane[i] = make_pair(L, R);
        if (found) {
            opposite_index.push_back({i, bound_index});
        }
    }

    cout << "Start counting...\n";
    int64_t res = 1LL * N * (N - 1) * (N - 2) / 6;

    for (int i = 0; i < tot_angles; i++) {
        int c_i = angles[i].count; // In ray i
        int s_i = cnt_lattices(i + 1, halfplane[i].first);
        res -= 1LL * c_i * (c_i - 1) * (c_i - 2) / 6; // Case A: All 3 points on ray i
        res -= 1LL * c_i * (c_i - 1) / 2 * s_i; // Case B: 2 points on ray i, 1 point in the strict half-plane
        res -= 1LL * c_i * s_i * (s_i - 1) / 2; // Case C: 1 point on ray i, 2 points in the strict half-plane
    }

    if ((int)opposite_index.size() > 0) {
        int64_t invalid_perfect = 0; // Need to declare a separate variable for duplicate counting, will divide by 2 later
        for (auto &[i, j]: opposite_index) {
            assert(angles[i].p == -angles[j].p);
            invalid_perfect += 1LL * angles[i].count * angles[j].count * (N - angles[i].count - angles[j].count); // Case D: 1 point on ray i, 1 point on ray j, 1 point anywhere else
            invalid_perfect += 1LL * angles[i].count * (angles[i].count - 1) / 2 * angles[j].count; // Case D: 2 points on ray i, 1 point on ray j
            invalid_perfect += 1LL * angles[j].count * (angles[j].count - 1) / 2 * angles[i].count; // Case D: 1 point on ray i, 2 points on ray j
        }

        assert(!(invalid_perfect & 1));
        res -= invalid_perfect / 2; // Each pair of opposite rays is counted twice in the loop, so we need to divide by 2
    }

    cout << res << '\n';
    return 0;
} 