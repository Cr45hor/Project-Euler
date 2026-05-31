#include <iostream>
#include <cmath>
#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>

using namespace std;

int const N = 100;

int spf[N + 1];
vector<int> primes;

void sieve() {
    for (int i = 1; i <= N; i++) {
        spf[i] = i;
    }

    for (int i = 2; i * i <= N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i) {
                spf[j] = i;
            }
        }
    }

    for (int i = 2; i <= N; i++) {
        if (spf[i] == i) {
            primes.push_back(i);
        }
    }
}

map<tuple<vector<int>, int, int>, int> max_mask;

int main() {
    sieve();

    cout << (int)primes.size() << '\n';

    int small_primes_cnt = count_if(primes.begin(), primes.end(), [](int p) {
        return 1LL * p * p <= N;
    });
    assert(small_primes_cnt < 86);

    for (int n = 2; n <= N; n++) {
        vector<int> mask;
        int big_prime = 1;
        int x = n;
        int m = 1;

        while(x > 1) {
            int p = spf[x];
            int id = lower_bound(primes.begin(), primes.end(), p) - primes.begin();
            if (id < small_primes_cnt) {
                mask.emplace_back(primes[id]);
            }
            else {
                // Only happens at most 1
                big_prime = p;
            }
            while(x % p == 0) {
                x /= p;
            }
            m *= p;
        }

        assert(mask.size() + (big_prime > 1) <= 6);

        auto it = max_mask.find({mask, m, big_prime});
        if (it == max_mask.end()) {
            max_mask[{mask, m, big_prime}] = n;
        }
        else {
            it->second = max(it->second, n);
        }

    }

    cout << max_mask.size() << '\n';

    // For each mask = {p1, p2, p3, ...}
    // Check if m < max_mask[{{p1}, p1, 1}] + max_mask[{{p2}, p2, 1}] + max_mask[{{p3}, p3, 1}] + ...
    // Basically, if the sum of the maximum power of each prime factors is greater than m, can remove that mask

    map<tuple<vector<int>, int, int>, int> next_mask;

    for (auto& [key, val] : max_mask) {
        auto& [mask, m, big_prime] = key;
        if (mask.size() + (big_prime > 1) <= 1) {
            next_mask[key] = val;
            continue;
        }

        int sum = 0;
        for (int p : mask) {
            int add = (1LL * p * p < N ? max_mask[{vector<int>({p}), p, 1}] : p);
            sum += add;
        }
        if (big_prime > 1) {
            sum += big_prime;
        }

        if (sum < m) {
            next_mask[key] = val;
        }
    }

    cout << next_mask.size() << '\n';
    for (auto& [key, val] : next_mask) {
        auto& [mask, m, big_prime] = key;
        cout << "mask: {";
        for (int p : mask) {
            cout << p << ' ';
        }
        cout << "}, m: " << m << ", big_prime: " << big_prime << ", max_n: " << val << '\n';
    }
}