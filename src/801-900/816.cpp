#include <iostream>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <set>
#include <iomanip>

using namespace std;

template <typename T> 
bool minimize(T &a, T b) { 
    if (a > b) { 
        return a = b, true;
    } 
    return false;
}

const int N = 2e6;
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

int64_t sq(int x) { return 1LL * x * x; }

pair<int, int> P[N];
set<pair<int, int>> sP;

int main() {
    PRNG prng(seed, MOD);

    for (int i = 0; i < N; i++) {
        P[i] = {prng.curr(), prng.next()};
        prng.next();
    }

    sort(P, P + N);
    int64_t mindis = 8LL * MOD * MOD;
 
    int prev = 0;
    sP.insert({P[0].second, P[0].first});
 
    for (int i = 1; i < N; i++) {
        int d = ceil(sqrt(mindis));
        while(prev < i && P[i].first - P[prev].first > d) {
            sP.erase({P[prev].second, P[prev].first});
            prev++;
        }

        auto it1 = sP.lower_bound(make_pair(P[i].second - d, P[i].first));
        auto it2 = sP.upper_bound(make_pair(P[i].second + d, P[i].first));
        for (auto it = it1; it != it2; it++) {
            int x = it ->second, y = it ->first;
            minimize(mindis, sq(x - P[i].first) + sq(y - P[i].second));
        }
        sP.insert({P[i].second, P[i].first});
    }
    cout << setprecision(9) << fixed << sqrt(mindis) << '\n';
    return 0;
}