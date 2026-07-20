#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

using namespace std;

const int N = 20000000;

int spf[N + 1];
vector<int> primes;

// Optimization 1: Strictly O(N) Linear Sieve
void linear_sieve() {
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > spf[i] || i * p > N) {
                break;
            }
            spf[i * p] = p;
        }
    }
}

int64_t extended_gcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t d = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

int I(int n) {
    if (n <= 2) return 0;

    // Separate n into 2^k (pow2) and the odd component (M)
    int temp = n;
    int pow2 = 1;
    while (temp % 2 == 0) {
        pow2 *= 2;
        temp /= 2;
    }
    int M = temp;

    // Extract prime powers for the odd component
    int odd_powers[8];
    int cnt = 0;
    while (temp > 1) {
        int p = spf[temp];
        int pk = 1;
        while (spf[temp] == p) {
            pk *= p;
            temp /= p;
        }
        odd_powers[cnt++] = pk;
    }

    // Precalculate all valid roots modulo 2^k
    int roots2[4];
    int num_roots2 = 0;
    if (pow2 <= 2) {
        roots2[num_roots2++] = 1;
    } else if (pow2 == 4) {
        roots2[num_roots2++] = 1;
        roots2[num_roots2++] = 3;
    } else {
        roots2[num_roots2++] = 1;
        roots2[num_roots2++] = pow2 / 2 - 1;
        roots2[num_roots2++] = pow2 / 2 + 1;
        roots2[num_roots2++] = pow2 - 1;
    }

    // Edge case: If n is purely a power of 2
    if (M == 1) {
        int max_m = 1;
        for (int i = 0; i < num_roots2; i++) {
            if (roots2[i] < n - 1) {
                max_m = max(max_m, roots2[i]);
            }
        }
        return max_m;
    }

    // Optimization 2: Compute Extended GCD for M and 2^k ONLY once per n
    int64_t u = 0, v = 0;
    if (pow2 > 1) {
        extended_gcd(M, pow2, u, v);
        u = (u % pow2 + pow2) % pow2;
        v = (v % M + M) % M;
    }

    int max_m = 1;
    int L = cnt;
    
    // Iterate through all subsets of odd prime powers
    for (int mask = 0; mask < (1 << L); mask++) {
        int P = 1, Q = 1;
        for (int i = 0; i < L; i++) {
            if ((mask >> i) & 1) {
                P *= odd_powers[i];
            } else {
                Q *= odd_powers[i];
            }
        }

        // Solve m_odd = 1 mod P and m_odd = -1 mod Q
        int64_t m_odd = 0;
        if (P == 1) {
            m_odd = M - 1;
        } else if (Q == 1) {
            m_odd = 1;
        } else {
            int64_t x, y;
            extended_gcd(P, Q, x, y);
            m_odd = (Q * y - P * x) % M;
            if (m_odd < 0) m_odd += M;
        }

        // Optimization 3: Combine m_odd with every root of 2^k using precalculated u, v
        for (int i = 0; i < num_roots2; i++) {
            int64_t m = m_odd;
            if (pow2 > 1) {
                m = ((int64_t)M * u % n * roots2[i] + (int64_t)pow2 * v % n * m_odd) % n;
            }
            if (m < n - 1 && m > max_m) {
                max_m = m;
            }
        }
    }
    
    return max_m;
}

int main() {
    linear_sieve();

    assert(I(15) == 11);
    assert(I(100) == 51);
    assert(I(7) == 1);

    int64_t res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int n = 3; n <= N; n++) {
        res += I(n);
    }
    
    cout << res << '\n';
    return 0;
}