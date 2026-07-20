#include <iostream>
#include <numeric>
#include <cstdint>
#include <cassert>
#include <vector>

using namespace std;

const int N = 1e8;
int mu[N + 1];
int pref_mu[N + 1];
vector<int> primes;
bool is_prime[N + 1];

void precompute() {
    fill(is_prime, is_prime + N + 1, true);
    is_prime[0] = is_prime[1] = false;
    mu[1] = 1;
    pref_mu[1] = 1;
    
    for (int i = 2; i <= N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (i * p > N) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            } else {
                mu[i * p] = -mu[i];
            }
        }
        pref_mu[i] = pref_mu[i - 1] + mu[i];
    }
}

int64_t H(int n) {
    int64_t tot = 1LL * (n - 1) * (n + 2) / 2;
    int64_t cnt = 0;
    
    // Count number of pairs (i, j) such that 1 <= i, j <= n, 1 <= i + j <= n, and gcd(i, j) = 1
    // Reduced to sum of mu(d) * S(floor(n / d)) for d = 1 to n, where S(k) = k * (k - 1) / 2

    // Block decomposition: compute in O(sqrt(n))
    for (int l = 1, r; l <= n; l = r + 1) {
        int v = n / l;
        r = n / v; // 'r' is the rightmost index that yields the same floor(n / l)
        
        int64_t s = 1LL * v * (v - 1) / 2;
        cnt += 1LL * (pref_mu[r] - pref_mu[l - 1]) * s;
    }
    
    return (tot - cnt) * 6;
}

int main() {
    precompute(); 
    cout << H(N) << '\n';
}