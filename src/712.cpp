#include <iostream>
#include <math.h>
#include <vector>
#include <bitset>
#include <assert.h>
#include <stdint.h>
#include <chrono>

using namespace std;

const int MAXV = 20000010;

const int MAXP = 7;
const int MAXN = 50;
const int MAXM = 2 * 3 * 7 * 5 * 11 * 13 * 17; /// Product of the first MAXP primes

constexpr auto fast_div = [](const long long& a, const int& b) ->long long {return double(a) / b + 1e-9;};

vector<int> primes;
bitset<MAXV> is_prime;
int prod[MAXP], pi[MAXV], dp[MAXN][MAXM];

void sieve(){
    is_prime[2] = true;
    for (int i = 3; i < MAXV; i += 2) is_prime[i] = true;

    for (int i = 3; i * i < MAXV; i += 2){
        for (int j = i * i; is_prime[i] && j < MAXV; j += (i << 1)){
            is_prime[j] = false;
        }
    }

    for (int i = 1; i < MAXV; i++){
        pi[i] = pi[i - 1] + is_prime[i];
        if (is_prime[i]) primes.push_back(i);
    }
}

void gen(){
    int i, j;
    assert(MAXN >= MAXP);

    sieve();
    for (prod[0] = primes[0], i = 1; i < MAXP; i++){
        prod[i] = prod[i - 1] * primes[i];
    }

    for (i = 0; i < MAXM; i++) dp[0][i] = i;
    for (i = 1; i < MAXN; i++){
        for (j = 1; j < MAXM; j++){
            dp[i][j] = dp[i - 1][j] - dp[i - 1][fast_div(j, primes[i - 1])];
        }
    }
}

uint64_t phi(long long m, int n){
    if (!n) return m;
    if (n < MAXN && m < MAXM) return dp[n][m];
    if (n < MAXP) return dp[n][m % prod[n - 1]] + fast_div(m, prod[n - 1]) * dp[n][prod[n - 1]];

    long long p = primes[n - 1];
    if (m < MAXV && p * p >= m) return pi[m] - n + 1;
    if (p * p * p < m || m >= MAXV) return phi(m, n - 1) - phi(fast_div(m, p), n - 1);

    int lim = pi[(int)sqrt(0.5 + m)];
    uint64_t res = pi[m] - (lim + n - 2) * (lim - n + 1) / 2;
    for (int i = n; i < lim; i++){
        res += pi[fast_div(m, primes[i])];
    }

    return res;
}

uint64_t lehmer(long long n){
    if (n < MAXV) return pi[n];

    int s = sqrt(0.5 + n), c = cbrt(0.5 + n);
    uint64_t res = phi(n, pi[c]) + pi[c] - 1;
    for (int i = pi[c]; i < pi[s]; i++){
        res -= lehmer(fast_div(n, primes[i])) - i;
    }

    return res;
}

int normalize(int x, int p) {
    if (x >= p) {
        x -= p;
    }
    else if (x < 0) {
        x += p;
    }
    return x;
}

int binpow(int a, int64_t n, int p) {
    int res = 1, base = a % p;
    for (; n; n >>= 1) {
        if (n & 1) 
            res = 1LL * res * base % p;
        base = 1LL * base * base % p;
    }
    return res;
}

int const MOD = 1e9 + 7;
struct Z{
    int x;
    Z(int x = 0): x(normalize(x, MOD)) {}
    Z(int64_t x): x(normalize(x % MOD, MOD)) {}

    Z operator-() const {
        return Z(-x);
    }
    Z inv() const {
        // Assume MOD is prime
        return Z(binpow(x, MOD - 2, MOD));
    }

    Z operator+(const Z& other) const {
        return Z(x + other.x);
    }
    Z operator-(const Z& other) const {
        return Z(x - other.x);
    }
    Z operator*(const Z& other) const {
        return Z(1LL * x * other.x);
    }
    Z operator/(const Z& other) const {
       return *this * other.inv();
    }

    Z& operator+=(const Z& other) {
        return *this = *this + other;
    }
    Z& operator-=(const Z& other) {
        return *this = *this - other;
    }
    Z& operator*=(const Z& other) {
        return *this = *this * other;
    }
    Z& operator/=(const Z& other) {
        return *this = *this / other;
    }

    bool operator==(const Z& other) const {
        return x == other.x;
    }

    friend ostream& operator<<(ostream& os, const Z& z) {
        return os << z.x;
    }
    friend istream& operator>>(istream& is, Z& z) {
        int64_t x;
        is >> x;
        z = Z(x);
        return is;
    }
};

int64_t N = 1e12;

int main() {
    // Time start
    auto start_time = chrono::high_resolution_clock::now();

    gen();

    int sqrt_N = (int)sqrt(0.5 + N);
    Z res = 0;

    for (int p = 1; p <= sqrt_N; p++) {
        if (is_prime[p]) {
            int64_t cnt_vp[64]{};
            int64_t pk = 1;
            int max_k = 0;
            for (int k = 0; k < 64; k++) {
                cnt_vp[k] = N / pk;
                if (pk > N / p) {
                    max_k = k;
                    break;
                }
                pk *= p;
                cnt_vp[k] -= N / pk;
            }
            assert(max_k < 64);

            for (int i = 0; i <= max_k; i++) {
                for (int j = i + 1; j <= max_k; j++) {
                    res += Z(cnt_vp[i]) * Z(cnt_vp[j]) * (j - i);
                }
            }
        }
    }

    for (int q = 1; q <= sqrt_N; q++) {
        int64_t L = max(N / (q + 1) + 1, (int64_t)sqrt_N + 1);
        int64_t R = N / q;
        if (L > R) continue;
        int64_t cnt_p = lehmer(R) - lehmer(L - 1);
        res += Z(cnt_p) * Z(q) * Z(N - q);
    }

    if ((int64_t)sqrt_N * sqrt_N == N && is_prime[sqrt_N]) {
        res -= Z(N % MOD) * Z(sqrt_N) - Z(1);
    }
    cout << res * 2 << endl;

    // Time end
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time taken: " << duration.count() << " ms" << endl;
    return 0;
}