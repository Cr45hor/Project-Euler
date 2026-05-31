#include <iostream>
#include <vector>
#include <cstdint>
#include <numeric>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

int const N = 12000;

map<pair<int, int>, vector<vector<int>>> factorization_cache;

// Unordered multiplicative factorizations
vector<vector<int>> get_factorizations(int n, int start_factor = 2) {
    if (factorization_cache.count({n, start_factor})) {
        return factorization_cache[{n, start_factor}];
    }

    vector<vector<int>> res = {{n}};
    for (int i = start_factor; i * i <= n; ++i) {
        if (n % i == 0) {
            for (auto sub : get_factorizations(n / i, i)) {
                vector<int> fact = {i};
                fact.insert(fact.end(), sub.begin(), sub.end());
                res.push_back(fact);
            }
        }
    }
    factorization_cache[{n, start_factor}] = res;
    return res;
}

int min_product_sum[N + 1];
int64_t res = 0;

int main() {
    for (int n = 2; n <= N * 2; ++n) {
        get_factorizations(n);
        bool found_any = false;
        for (const auto& fact : factorization_cache[{n, 2}]) {
            int sum = 0;
            for (size_t i = 0; i < fact.size(); ++i) {
                sum += fact[i];
            }
            int k = fact.size() + (n - sum);
            if (k >= 2 && k <= N && min_product_sum[k] == 0) {
                min_product_sum[k] = n;
                found_any = true;
            }
        }
        if (found_any) {
            res += n;
        }
    }

    assert(all_of(min_product_sum + 2, min_product_sum + N + 1, [](int x) { return x > 0; }));
    cout << res << '\n';
}