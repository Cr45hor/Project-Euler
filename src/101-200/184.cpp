#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

struct Point {
    int x = 0, y = 0;
    Point() {}
    Point(int __x, int __y): x(__x), y(__y) {}
    
    Point operator-() const { return Point(-x, -y); }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

// Bundle the Point with its count to eliminate the need for std::map
struct Ray {
    Point p;
    int count;
};

const int R = 105;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Build Quadrant 1 [0, pi/2)
    // We explicitly start x at 1 to skip the Y-axis. 
    // This removes the need to use lattice_pts.pop_back() later, because rotating
    // the X-axis by 90 degrees will naturally and perfectly generate the Y-axis for us.
    int q1_cnt[R + 1][R + 1] = {0};
    vector<Point> q1_pts;
    
    for (int x = 1; x <= R; ++x) { 
        for (int y = 0; y <= R; ++y) {
            if (x * x + y * y < R * R) {
                int g = std::gcd(x, y);
                int dx = x / g, dy = y / g;
                if (q1_cnt[dx][dy] == 0) {
                    q1_pts.push_back(Point(dx, dy));
                }
                q1_cnt[dx][dy]++;
            }
        }
    }

    // Sort by exact integer cross-product instead of atan2
    sort(q1_pts.begin(), q1_pts.end(), [](const Point& a, const Point& b) {
        return a.x * b.y - a.y * b.x > 0;
    });

    // 2. Bundle points with their counts
    vector<Ray> angles;
    angles.reserve(q1_pts.size() * 8); // Pre-allocate memory for speed
    for (const Point& p : q1_pts) {
        angles.push_back({p, q1_cnt[p.x][p.y]});
    }

    int tot_angles = angles.size();

    // 3. Duplicate through all 4 quadrants
    for (int w = 1; w < 4; w++) {
        int index_l = (w - 1) * tot_angles;
        int index_r = w * tot_angles;
        for (int i = index_l; i < index_r; i++) {
            Ray r = angles[i];
            // Rotate 90 degrees counter-clockwise and preserve the count
            angles.push_back({Point(-r.p.y, r.p.x), r.count});
        }
    }

    // Duplicate all angles for circular wrap-around
    assert((int)angles.size() == tot_angles * 4);
    for (int i = 0; i < tot_angles * 4; i++) {
        angles.push_back(angles[i]);
        assert(angles[i].p == angles[i + tot_angles * 4].p);
    }

    // 4. Prefix Sum Arrays 
    // Using tightly-sized dynamic vectors instead of massive R*R*8 raw arrays on the stack
    int total_size = tot_angles * 8;
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

    // 5. Counting Logic
    // Replacing std::function with auto lambdas for inline optimization
    auto cnt_lattices = [&](int l, int r) -> int {
        if (l > r) return 0;
        return sum_pts[r] - (l == 0 ? 0 : sum_pts[l - 1]);
    };

    auto cnt_prod = [&](int l, int r, int type) -> int64_t {
        if (l > r) return 0LL;
        return sum_prod[type][r] - (l == 0 ? 0 : sum_prod[type][l - 1]);
    };

    cout << "Start counting...\n";
    int64_t res = 0;
    for (int i = 0; i < tot_angles * 4; i++) {
        assert(angles[i].p == angles[i + tot_angles * 4].p);
        assert(angles[i].p == -angles[i + tot_angles * 2].p);

        // Your exact equations
        int64_t res_1 = 1LL * cnt_prod(i, i + tot_angles * 2 - 2, 1) - 1LL * sum_pts[i] * cnt_lattices(i + 1, i + tot_angles * 2 - 1);
        int64_t res_2 = 1LL * sum_pts[i + tot_angles * 4 - 1] * cnt_lattices(i + tot_angles * 2 + 1, i + tot_angles * 4 - 1) - cnt_prod(i + tot_angles * 2 + 1, i + tot_angles * 4 - 1, 0);
        
        res += 1LL * angles[i].count * (res_1 + res_2);
    }
    
    cout << res / 6 << '\n';

    return 0;
}