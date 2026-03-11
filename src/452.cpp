#include <iostream>
#include <numeric>

using namespace std;

using u64 = long long;

int const N = 30;
int const M = 1 << N;
int const MOD = 1234567891; // prime number

int norm(u64 x) {
    if (x >= MOD) x -= MOD;
    else if (x < 0) x += MOD;
    return x;
}

int add(int x, int y) {
    return norm((u64)x + (u64)y);
}

int mul(int x, int y) {
    return norm(1LL * x * y % MOD);
}

int* F;
int sum_row[N];

int& val(int r, int id) {
    return F[(u64)id + r * M];
}

// Modular exponentiation: (a^b) % p
long long mod_pow(long long a, long long b, long long p) {
    long long result = 1;
    a %= p;
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % p;
        }
        a = (a * a) % p;
        b >>= 1;
    }
    return result;
}

// Modular inverse using Fermat's Little Theorem
long long mod_inv(long long a, long long p) {
    return mod_pow(a, p - 2, p);
}

// Calculate C(n, k) mod p for small k
long long binomial_coefficient(long long n, long long k, long long p) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    // Since k is small, we can compute numerator and denominator directly
    long long numerator = 1;
    long long denominator = 1;
    
    // Compute numerator: n × (n-1) × ... × (n-k+1) mod p
    for (long long i = 0; i < k; i++) {
        numerator = (numerator * ((n - i) % p)) % p;
    }
    
    // Compute denominator: k! mod p
    for (long long i = 1; i <= k; i++) {
        denominator = (denominator * i) % p;
    }
    
    // C(n, k) = numerator × denominator^(-1) mod p
    return (numerator * mod_inv(denominator, p)) % p;
}

int main() {
    int const n = 1e9;
    int const m = 1e9;

    F = new int[(u64)M << 1];

    val(0, 1) = 1;
    int cur_row = 0;
    for (int len = 0; len < n && (1 << len) < m; len++) {
        for (int prod = 1; prod <= m; prod++) {
            sum_row[len] = add(sum_row[len], val(cur_row, prod));
            val(cur_row ^ 1, prod) = 0;
        }

        for (int prod = 1; prod <= m; prod++) {
            if (val(cur_row, prod) == 0) {
                continue;
            }
            
            int max_multiply = m / prod;
            for (int x = 2; x <= max_multiply; x++) {
                val(cur_row ^ 1, prod * x) = add(val(cur_row ^ 1, prod * x), val(cur_row, prod));
            }
        }
        
        cout << len << ' ' << sum_row[len] << '\n';
        cur_row ^= 1;
    }

    int result = 0;
    for (int len = 0; len <= n && (1 << len) <= m; len++) {
        result = add(result, mul(sum_row[len], binomial_coefficient(n, len, MOD)));
    }
    cout << result;
    
    return 0;
}