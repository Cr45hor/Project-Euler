#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <execution> 

using namespace std;

struct RNG_gen {
    int seed = 1;
    int M = 1;
    int offset = 0;
    int current_index = 0;

    RNG_gen(int _seed, int _M): seed(_seed), M(_M) {
        offset = (1 - M) / 2;
    }

    int prod = 1;
    void update() { prod = (1LL * prod * seed) % M; }
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

    int quad() const {
        assert(!(x == 0 && y == 0));
        if (x > 0 && y >= 0) return 1;
        if (x <= 0 && y > 0) return 2;
        if (x < 0 && y <= 0) return 3;
        if (x >= 0 && y < 0) return 4;
        assert(false);
    }
};

int32_t ccw(const Point& a, const Point& b) {
    int64_t cross = 1LL * a.x * b.y - 1LL * a.y * b.x;
    if (cross > 0) return 1;
    if (cross < 0) return -1;
    return 0;
}

bool cmp_ccw(const Point& a, const Point& b) {
    if (a.quad() != b.quad()) return a.quad() < b.quad();
    return ccw(a, b) > 0;
}

struct Ray {
    Point p;
    int count;
};

int const N = 2e6;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    RNG_gen X(1248, 32323);
    RNG_gen Y(8421, 30103);

    // 1. Data-Oriented Point Generation (No std::map)
    vector<Point> raw_pts;
    raw_pts.reserve(N);
    for (int i = 1; i <= N; i++) {
        int x = X.next();
        int y = Y.next();
        if (x == 0 && y == 0) continue;
        
        int g = std::gcd(std::abs(x), std::abs(y));
        raw_pts.push_back(Point(x / g, y / g));
    }

    // 2. Sort all raw points radially
    // NOTE: If you want to use parallel computing, this is where you do it.
    // Replace with: 
    // sort(std::execution::par_unseq, raw_pts.begin(), raw_pts.end(), cmp_ccw);
    sort(raw_pts.begin(), raw_pts.end(), cmp_ccw);

    // 3. Run-Length Encoding to bundle unique rays and their counts
    vector<Ray> angles;
    for (const auto& p : raw_pts) {
        if (!angles.empty() && angles.back().p == p) {
            angles.back().count++;
        } else {
            angles.push_back({p, 1});
        }
    }

    int tot_angles = angles.size();

    // Duplicate all angles for circular wrap-around
    angles.reserve(tot_angles * 2);
    for (int i = 0; i < tot_angles; i++) {
        angles.push_back(angles[i]);
    }

    // 4. Prefix Sum Arrays
    int total_size = tot_angles * 2;
    vector<int> sum_pts(total_size, 0);

    sum_pts[0] = angles[0].count;
    for (int i = 1; i < total_size; i++) {
        sum_pts[i] = sum_pts[i - 1] + angles[i].count;
    }

    auto cnt_lattices = [&](int l, int r) -> int {
        if (l > r) return 0;
        return sum_pts[r] - (l == 0 ? 0 : sum_pts[l - 1]);
    };

    // 5. Sliding Window (Two Pointers) replacing Binary Search
    vector<pair<int, int>> halfplane(tot_angles);
    vector<pair<int, int>> opposite_index;
    
    int j = 0;
    for (int i = 0; i < tot_angles; i++) {
        j = max(j, i + 1); // j must always lead i
        
        // Advance j as long as it remains strictly in the open counter-clockwise half-plane
        while (j < i + tot_angles && ccw(angles[i].p, angles[j].p) > 0) {
            j++;
        }
        
        // j is now the first index outside the open half-plane.
        // Therefore, the strict half-plane ends at j - 1.
        halfplane[i] = make_pair(j - 1, j);
        
        // If j hasn't wrapped completely around, check if it is the exact opposite ray
        if (j < i + tot_angles && angles[i].p == -angles[j].p) {
            opposite_index.push_back({i, j});
        }
    }

    cout << "Start counting...\n";
    int64_t res = 1LL * N * (N - 1) * (N - 2) / 6; 

    // 6. Complement Counting (Your Exact Equations)
    for (int i = 0; i < tot_angles; i++) {
        int c_i = angles[i].count; 
        int s_i = cnt_lattices(i + 1, halfplane[i].first);

        res -= 1LL * c_i * (c_i - 1) * (c_i - 2) / 6; 
        res -= 1LL * c_i * (c_i - 1) / 2 * s_i; 
        res -= 1LL * c_i * s_i * (s_i - 1) / 2; 
    }

    if (!opposite_index.empty()) {
        int64_t invalid_perfect = 0; 
        for (auto &[i, opp_j]: opposite_index) {
            invalid_perfect += 1LL * angles[i].count * angles[opp_j].count * (N - angles[i].count - angles[opp_j].count); 
            invalid_perfect += 1LL * angles[i].count * (angles[i].count - 1) / 2 * angles[opp_j].count; 
            invalid_perfect += 1LL * angles[opp_j].count * (angles[opp_j].count - 1) / 2 * angles[i].count; 
        }
        res -= invalid_perfect / 2; 
    }
    
    cout << res << '\n';

    return 0;
}