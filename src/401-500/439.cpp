#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>

using namespace std;

const int MOD = 1e9; // Trap avoided: 10^9 is not prime.
const int64_t N = 100000000000LL; 
const int MAXM = 5000000;

// Helper to handle negative numbers safely
int normalize(int64_t X, int mod) {
    X %= mod;
    if (X < 0) X += mod;
    return X;
}

// Modular integer class for arithmetic modulo MOD
struct Z {
    int x;
    Z(int X = 0) : x(normalize(X, MOD)) {}
    Z(int64_t X) : x(normalize(X, MOD)) {}

    Z operator-() const {
        return Z(-x);
    }
    
    // .inv() and operator/ are INTENTIONALLY REMOVED. 
    // They cannot exist when MOD is not prime.

    Z operator+(const Z& other) const { return Z(x + other.x); }
    Z operator-(const Z& other) const { return Z(x - other.x); }
    Z operator*(const Z& other) const { return Z(1LL * x * other.x); }

    Z& operator+=(const Z& other) { return *this = *this + other; }
    Z& operator-=(const Z& other) { return *this = *this - other; }
    Z& operator*=(const Z& other) { return *this = *this * other; }
};

// Sieve arrays
vector<int> primes;
vector<bool> np;
vector<int> mu;
vector<long long> pk;       // stores p^k
vector<long long> sig_pk;   // stores sigma(p^k)
vector<long long> sig;      // stores sigma(i)
vector<Z> M1;
vector<Z> F;

void sieve() {
    np.assign(MAXM + 1, false);
    mu.assign(MAXM + 1, 0);
    pk.assign(MAXM + 1, 0);
    sig_pk.assign(MAXM + 1, 0);
    sig.assign(MAXM + 1, 0);
    M1.assign(MAXM + 1, 0);
    F.assign(MAXM + 1, 0);

    mu[1] = 1;
    pk[1] = 1;
    sig_pk[1] = 1;
    sig[1] = 1;

    // Linear Sieve to compute multiplicative functions in O(M)
    for (int i = 2; i <= MAXM; ++i) {
        if (!np[i]) {
            primes.push_back(i);
            mu[i] = -1;
            pk[i] = i;
            sig_pk[i] = i + 1;
            sig[i] = i + 1;
        }
        for (int p : primes) {
            if (i * p > MAXM) break;
            np[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                pk[i * p] = pk[i] * p;
                sig_pk[i * p] = sig_pk[i] + pk[i * p];
                sig[i * p] = sig[i / pk[i]] * sig_pk[i * p];
                break;
            } else {
                mu[i * p] = -mu[i];
                pk[i * p] = p;
                sig_pk[i * p] = p + 1;
                sig[i * p] = sig[i] * (p + 1);
            }
        }
    }

    Z cur_M1 = 0;
    Z cur_F = 0;
    for (int i = 1; i <= MAXM; ++i) {
        cur_M1 += Z(i) * Z(mu[i]);
        M1[i] = cur_M1;
        
        cur_F += Z(sig[i]);
        F[i] = cur_F;
    }
}

// Safe sum of range [L, R] that divides by 2 BEFORE modulo
Z sum_range(int64_t L, int64_t R) {
    int64_t a = L + R;
    int64_t b = R - L + 1;
    if (a % 2 == 0) a /= 2;
    else b /= 2;
    return Z(a) * Z(b);
}

// Dirichlet Prefix Sum Memoization
unordered_map<int64_t, Z> memo_M1;
Z get_M1(int64_t K) {
    if (K <= MAXM) return M1[K];
    if (memo_M1.count(K)) return memo_M1[K];
    
    Z res = 1;
    for (int64_t l = 2, r; l <= K; l = r + 1) {
        int64_t v = K / l;
        r = K / v;
        res -= sum_range(l, r) * get_M1(v);
    }
    return memo_M1[K] = res;
}

unordered_map<int64_t, Z> memo_F;
Z get_F(int64_t K) {
    if (K <= MAXM) return F[K];
    if (memo_F.count(K)) return memo_F[K];
    
    Z res = 0;
    for (int64_t l = 1, r; l <= K; l = r + 1) {
        int64_t v = K / l;
        r = K / v;
        res += sum_range(l, r) * Z(v);
    }
    return memo_F[K] = res;
}

// Final Block-Sum integration
Z get_S(int64_t N_val) {
    Z ans = 0;
    for (int64_t l = 1, r; l <= N_val; l = r + 1) {
        int64_t v = N_val / l;
        r = N_val / v;
        
        Z term = get_M1(r) - get_M1(l - 1);
        Z f_v = get_F(v);
        
        ans += term * f_v * f_v;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    sieve();
    cout << get_S(N).x << "\n";
    
    return 0;
}