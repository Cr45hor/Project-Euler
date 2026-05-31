#include <iostream>
#include <vector>
#include <map>

using namespace std;

int const N = 1e6;

int is_prime[N];
vector<int> primes;

void sieve() {
    for (int i = 2; i < N; i++) {
        is_prime[i] = 1;
    }
    for (int i = 2; i * i < N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    for (int p = 2; p < N; p++) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}


long long sum_prime[N];
map<long long, int> sums;

int main() {
    sieve();

    sums[0] = 0;
    for (int i = 0; i < (int)primes.size(); i++) {
        sum_prime[i + 1] = sum_prime[i] + primes[i];
        sums[sum_prime[i + 1]] = i + 1;
    }

    pair<int, int> ans = {0, 0};
    for (int i = 0; i < (int)primes.size(); i++) {
        for (int j = i + ans.first + 1; j < (int)primes.size(); j++) {
            long long sum = sum_prime[j] - sum_prime[i];
            if (sum >= N) {
                break;
            }
            if (is_prime[sum]) {
                ans = max(ans, {j - i, (int)sum});
            }
        }
    }
    cout << ans.second << '\n';
}