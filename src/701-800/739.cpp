#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const long long MOD = 1000000007;
const long long N = 100000000;
const int CHUNK_SIZE = 100000;

// Safe modulo operation to handle C++'s negative modulo behavior
inline long long safe_mod(long long x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return x;
}

// Fast modular exponentiation
long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

// Computes modular inverse of an array of numbers using Montgomery's trick
vector<long long> batch_inverse(const vector<long long>& arr) {
    int n = arr.size();
    if (n == 0) return {};
    
    vector<long long> pref(n, 1);
    pref[0] = arr[0];
    for (int i = 1; i < n; i++) {
        pref[i] = (pref[i - 1] * arr[i]) % MOD;
    }
    
    long long inv_prod = power(pref[n - 1], MOD - 2);
    vector<long long> inv_arr(n, 0);
    
    for (int i = n - 1; i > 0; i--) {
        inv_arr[i] = (inv_prod * pref[i - 1]) % MOD;
        inv_prod = (inv_prod * arr[i]) % MOD;
    }
    inv_arr[0] = inv_prod;
    
    return inv_arr;
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();
    
    long long m = N - 1;
    if (m == 0) {
        cout << 0 << "\n";
        return 0;
    }
    
    // 1. Compute a_m
    long long a_m = 0;
    if (m < 4) {
        long long init_a[] = { -2, -2, -9, -38 };
        a_m = safe_mod(init_a[m]);
    } else {
        long long a_prev = safe_mod(-9);
        long long a_curr = safe_mod(-38);
        for (long long i = 4; i <= m; i++) {
            long long next_a = safe_mod(4 * a_curr + a_prev);
            a_prev = a_curr;
            a_curr = next_a;
        }
        a_m = a_curr;
    }
    
    // 2. Compute b_m
    long long b_m = 0;
    if (m < 4) {
        long long init_b[] = { 2, 5, 16, 59 };
        b_m = init_b[m];
    } else {
        long long b0 = 2, b1 = 5, b2 = 16, b3 = 59;
        vector<long long> arr;
        arr.reserve(CHUNK_SIZE);
        
        for (long long chunk_start = 4; chunk_start <= m; chunk_start += CHUNK_SIZE) {
            long long chunk_end = min(m + 1, chunk_start + CHUNK_SIZE);
            arr.clear();
            
            for (long long i = chunk_start; i < chunk_end; i++) {
                arr.push_back(i);
            }
            
            vector<long long> inv_arr = batch_inverse(arr);
            
            for (size_t i = 0; i < arr.size(); i++) {
                long long n = arr[i];
                
                // Safely compute the recurrence terms to avoid overflow and negative mod issues
                long long t1 = safe_mod(15 * n - 10) * b3 % MOD;
                long long t2 = safe_mod(22 * n - 26) * b2 % MOD;
                long long t3 = safe_mod(23 * n - 66) * b1 % MOD;
                long long t4 = safe_mod(4 * n - 14) * b0 % MOD;
                
                long long term = safe_mod(t1 - t2 - t3 - t4);
                
                // Divide by 2 modulo MOD
                if (term % 2 != 0) term += MOD;
                term /= 2;
                
                // Multiply by the inverse of n
                long long bn = (term * inv_arr[i]) % MOD;
                
                // Shift states
                b0 = b1; b1 = b2; b2 = b3; b3 = bn;
            }
        }
        b_m = b3;
    }
    
    // 3. Final Answer
    long long res = safe_mod(a_m + b_m);
    
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    
    cout << "Result: " << res << "\n";
    cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    
    return 0;
}