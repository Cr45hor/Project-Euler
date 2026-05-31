#include <iostream>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>

using namespace std;

const int MAX_PRIME = 1005000;

int spf[MAX_PRIME + 1];
int phi[MAX_PRIME + 1];
vector<int> primes;

void sieve() {
    for (int i = 1; i <= MAX_PRIME; i += 2) {
        spf[i] = i;
    }
    for (int i = 2; i <= MAX_PRIME; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i <= MAX_PRIME; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_PRIME; j += 2 * i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    for (int n = 2; n <= MAX_PRIME; n++) {
        if (spf[n] == n) {
            primes.push_back(n);
            phi[n] = n - 1;
        } else {
            int p = spf[n];
            int m = n / p;
            if (m % p == 0) {
                phi[n] = phi[m] * p;
            } else {
                phi[n] = phi[m] * (p - 1);
            }
        }
    }
}

int factorize(int n, int buffer[8 * 3]) {
    assert(1 <= n && n <= MAX_PRIME);
    int omega = 0;
    while(n > 1) {
        int p = spf[n];
        int k = 0;
        int pk = 1;
        while(spf[n] == p) {
            n /= p;
            k++;
            pk *= p;
        }
        buffer[omega * 3] = p;
        buffer[omega * 3 + 1] = k;
        buffer[omega * 3 + 2] = pk;
        omega++;
    }
    return omega;
}

vector<tuple<int, int, int>> decompose_eq(int a, int n) {
    // Decompose x = a (mod n) into x = a_i (mod p_i^k_i)
    int buffer[8 * 3];
    int omega = factorize(n, buffer);
    vector<tuple<int, int, int>> res;
    for (int i = 0; i < omega; i++) {
        int p = buffer[i * 3];
        int pk = buffer[i * 3 + 2];
        res.emplace_back(a % pk, pk, p);
    }
    return res;
}

int64_t extended_gcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t d = extended_gcd(b, a % b, y, x);
    y -= (a / b) * x;
    return d;
}

int64_t crt(vector<pair<int, int>>& eq) {
    int64_t N = 1;
    for (auto& [_, n_i]: eq) {
        N *= n_i;
    }

    __int128_t res = 0;
    __int128_t N_128 = N;
    for (auto &[a_i, n_i]: eq) {
        int64_t N_i = N / n_i;
        int64_t M_i, m_i;
        extended_gcd(N_i, n_i, M_i, m_i);
        
        // M_i can be negative, so term can be negative.
        // We do modulo multiplication in steps to stay within bounds.
        __int128_t term = (__int128_t)a_i * M_i % N_128;
        term = (term * N_i) % N_128;
        res = (res + term) % N_128;
    }
    
    // In C++, modulo of negative numbers is negative. Ensure positive result.
    if (res < 0) {
        res += N_128;
    }
    return (int64_t)res;
}

int64_t g(int64_t a, int64_t n, int64_t b, int64_t m) {
    int64_t M, N;
    int64_t d = extended_gcd(n, m, M, N);
    
    // Condition for a solution to exist
    if ((b - a) % d != 0) {
        return 0; 
    }
    
    int64_t L = (n / d) * m; // lcm(n, m)
    
    // Calculate the term: k * M * n
    // k and M can be negative, so we use __int128_t to handle signs
    // and prevent any multiplication overflow before modulo.
    __int128_t k = (b - a) / d;
    
    // Modulo L at each multiplication step keeps values small
    __int128_t term = (k * M % L) * n % L;
    __int128_t x = (a + term) % L;
    
    // Ensure the result is positive
    if (x < 0) {
        x += L;
    }
    
    return (int64_t)x;
}

int64_t f(int n, int m) {
    return g(phi[n], n, phi[m], m);
}


const int L = 1e6;
const int R = L + 5000;

int main() {
    sieve();

    int64_t res = 0;
    for (int n = L; n < R; n++) {
        for (int m = n + 1; m < R; m++) {
            res += f(n, m);
        }
    }
    cout << res << '\n';
}