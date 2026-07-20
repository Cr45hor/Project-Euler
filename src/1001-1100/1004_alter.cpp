#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const long long MOD = 1e9 + 7;

long long fact[105];
long long total_balanced_len[105];
long long total_starting_zero_len[105];

// Standard modular exponentiation
long long modPow(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

// Modular inverse via Fermat's Little Theorem
long long modInverse(long long n) {
    return modPow(n, MOD - 2);
}

// Precompute factorials for the Hook-Length formula numerator
void precompute() {
    fact[0] = 1;
    for (int i = 1; i <= 100; ++i) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
}

// Computes the number of valid sequences that map to a specific Young Diagram shape 'p'
long long count_sequences(const vector<int>& p) {
    int n = 0;
    for (int x : p) n += x;
    if (n == 0) return 1;

    // Calculate column heights (conjugate partition)
    vector<int> col_len(p[0], 0);
    for (size_t i = 0; i < p.size(); ++i) {
        for (int j = 0; j < p[i]; ++j) {
            col_len[j]++;
        }
    }

    long long num = fact[n];
    long long den = 1;

    // Apply the Hook-Length and Hook-Content formulas
    for (int i = 0; i < p.size(); ++i) {
        for (int j = 0; j < p[i]; ++j) {
            int h = p[i] + col_len[j] - i - j - 1; // Hook length
            int content_term = 10 + j - i;         // Content bounded by alphabet size 10
            
            num = (num * content_term) % MOD;
            den = (den * (1LL * h * h % MOD)) % MOD;
        }
    }
    
    return (num * modInverse(den)) % MOD;
}

// Recursive generator for all integer partitions fitting in a 10x10 grid
void generate_partitions(int max_width, int max_height, vector<int>& p) {
    if (!p.empty()) {
        int w = p[0];
        int h = p.size();
        int n = 0; 
        for (int x : p) n += x;
        
        // Condition 1: Fully balanced shape (LNDS == LDS)
        if (w == h) {
            total_balanced_len[n] = (total_balanced_len[n] + count_sequences(p)) % MOD;
        }
        
        // Condition 2: "Almost balanced" shape mapping to sequences starting with '0'
        if (h == w + 1) {
            total_starting_zero_len[n + 1] = (total_starting_zero_len[n + 1] + count_sequences(p)) % MOD;
        }
    }

    if (p.size() == max_height) return;

    // Maintain non-increasing order to form valid partitions
    int limit = p.empty() ? max_width : p.back();
    for (int i = limit; i >= 1; --i) {
        p.push_back(i);
        generate_partitions(max_width, max_height, p);
        p.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    auto start_time = chrono::high_resolution_clock::now();

    precompute();
    
    // The single '0' is technically a balanced sequence of length 1, 
    // but we exclude it because valid integers don't have leading zeros.
    total_starting_zero_len[1] = 1;

    vector<int> current_partition;
    generate_partitions(10, 10, current_partition);

    long long cumulative_balanced = 0;
    
    // Output format mirroring the DP trace
    for (int length = 1; length <= 100; ++length) {
        long long exact_for_length = (total_balanced_len[length] - total_starting_zero_len[length] + MOD) % MOD;
        cumulative_balanced = (cumulative_balanced + exact_for_length) % MOD;
        
        // To prevent console spam, let's just print the first few and the final answer
        if (length <= 8 || length == 100) {
            cout << "Length " << length << ": Total balanced numbers so far = " << cumulative_balanced << "\n";
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end_time - start_time;
    
    cout << "\nExecution time: " << elapsed.count() << " ms\n";

    return 0;
}