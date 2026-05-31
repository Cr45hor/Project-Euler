#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cstdint>

using namespace std;

struct Point {
    int x, y;
};

// Assign quadrants 1 to 4 in Counter-Clockwise order to avoid atan2
int quad(Point p) {
    if (p.x > 0 && p.y >= 0) return 1;
    if (p.x <= 0 && p.y > 0) return 2;
    if (p.x < 0 && p.y <= 0) return 3;
    if (p.x >= 0 && p.y < 0) return 4;
    return 0;
}

// Compare by quadrant first, then by 2D cross product for exact CCW ordering
bool cmp(Point a, Point b) {
    if (quad(a) != quad(b)) return quad(a) < quad(b);
    return a.x * b.y - a.y * b.x > 0;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int R = 105;
    // 250x250 array is enough to cover all possible [-105, 105] variations with a 125 offset
    int counts[250][250] = {0}; 
    vector<Point> rays;
    int64_t n = 0; // Total valid lattice points

    // 1. Gather points and unique rays
    for (int x = -R; x <= R; ++x) {
        for (int y = -R; y <= R; ++y) {
            if (x * x + y * y < R * R && (x != 0 || y != 0)) {
                n++;
                int g = std::gcd(std::abs(x), std::abs(y));
                int dx = x / g;
                int dy = y / g;
                
                if (counts[dx + 125][dy + 125] == 0) {
                    rays.push_back({dx, dy});
                }
                counts[dx + 125][dy + 125]++;
            }
        }
    }

    // 2. Sort radially
    sort(rays.begin(), rays.end(), cmp);

    int K = rays.size();
    
    // 3. Build circular array and prefix sums
    vector<int64_t> C(2 * K);
    for (int i = 0; i < 2 * K; ++i) {
        C[i] = counts[rays[i % K].x + 125][rays[i % K].y + 125];
    }

    vector<int64_t> pref(2 * K + 1, 0);
    for (int i = 0; i < 2 * K; ++i) {
        pref[i + 1] = pref[i] + C[i];
    }

    // 4. Complement Counting
    int64_t total_triangles = n * (n - 1) * (n - 2) / 6;
    int64_t invalid = 0;

    for (int i = 0; i < K; ++i) {
        int64_t c_i = C[i];
        // Sum of points strictly in the open CCW half-plane (i, i + pi)
        int64_t s_i = pref[i + K / 2] - pref[i + 1]; 

        // Invalid Case A: All 3 points on ray i
        invalid += c_i * (c_i - 1) * (c_i - 2) / 6;
        // Invalid Case B: 2 points on ray i, 1 point in the strict half-plane
        invalid += c_i * (c_i - 1) / 2 * s_i;
        // Invalid Case C: 1 point on ray i, 2 points in the strict half-plane
        invalid += c_i * s_i * (s_i - 1) / 2;
    }

    // Since the rays are perfectly symmetric, K is guaranteed to be even.
    // We only need to iterate half the rays to process exact opposite pairs.
    for (int i = 0; i < K / 2; ++i) {
        int64_t c_i = C[i];
        int64_t c_j = C[i + K / 2]; // The ray exactly opposite to i
        
        int64_t pairs_i = c_i * (c_i - 1) / 2;
        int64_t pairs_j = c_j * (c_j - 1) / 2;
        
        // Invalid Case D: Max angle is exactly pi (origin sits on the boundary of the triangle)
        // Includes: 2 on i & 1 on j, 1 on i & 2 on j, and (1 on i & 1 on j & 1 anywhere else)
        invalid += pairs_i * c_j + c_i * pairs_j + c_i * c_j * (n - c_i - c_j);
    }

    int64_t ans = total_triangles - invalid;
    
    cout << "Start counting...\n";
    cout << ans << '\n'; // No need to divide by 6, we generated true combinatorics.

    return 0;
}