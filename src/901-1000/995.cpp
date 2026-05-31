#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>
#include <gmpxx.h> // GMP C++ wrapper

using namespace std;

const int MAX_PRIME_SIEVE = 2000000;
const string CACHE_FILE = "min_s_cache.txt";

// Fast modular exponentiation
long long pow_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

// Sieve arrays
vector<bool> is_p_sieve;
vector<int> primes;

void build_sieve() {
    is_p_sieve.assign(MAX_PRIME_SIEVE, true);
    is_p_sieve[0] = is_p_sieve[1] = false;
    for (int i = 2; i * i < MAX_PRIME_SIEVE; i++) {
        if (is_p_sieve[i]) {
            for (int j = i * i; j < MAX_PRIME_SIEVE; j += i) {
                is_p_sieve[j] = false;
            }
        }
    }
    for (int i = 2; i < MAX_PRIME_SIEVE; i++) {
        if (is_p_sieve[i]) primes.push_back(i);
    }
}

// Fallback for primes larger than 2,000,000
bool is_prime_dynamic(long long n) {
    if (n < MAX_PRIME_SIEVE) return is_p_sieve[n];
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Unordered multiplicative factorizations
vector<vector<int>> get_factorizations(int n, int start_factor = 2) {
    vector<vector<int>> res = {{n}};
    for (int i = start_factor; i * i <= n; ++i) {
        if (n % i == 0) {
            for (auto sub : get_factorizations(n / i, i)) {
                vector<int> fact = {i};
                fact.insert(fact.end(), sub.begin(), sub.end());
                res.push_back(fact);
            }
        }
    }
    return res;
}

// Find smallest primitive root generator for p
int primitive_root(int p) {
    if (p == 2) return 1;
    int phi = p - 1;
    vector<int> factors;
    int n = phi;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors.push_back(n);

    for (int res = 2; res < p; ++res) {
        bool valid = true;
        for (int f : factors) {
            if (pow_mod(res, phi / f, p) == 1) {
                valid = false;
                break;
            }
        }
        if (valid) return res;
    }
    return -1;
}

// Load cache from disk
map<int, string> load_cache() {
    map<int, string> cache;
    ifstream infile(CACHE_FILE);
    string line;
    while (getline(infile, line)) {
        size_t colon_pos = line.find(':');
        if (colon_pos != string::npos) {
            int p = stoi(line.substr(0, colon_pos));
            string s_val = line.substr(colon_pos + 1);
            // strip whitespace
            s_val.erase(remove_if(s_val.begin(), s_val.end(), ::isspace), s_val.end());
            cache[p] = s_val;
        }
    }
    return cache;
}

// Append new result to cache
void save_to_cache(int p, const mpz_class& s) {
    ofstream outfile(CACHE_FILE, ios_base::app);
    outfile << p << ": " << s.get_str() << "\n";
}

// The core algorithm powered by GMP mpz_class
mpz_class find_minimum_s(int p) {
    if (p == 2) return 1;
    
    int g = primitive_root(p);
    vector<vector<int>> factorizations = get_factorizations(p - 1);
    
    // O(1) Pre-bucket
    vector<int> first_prime_for_mod(p, 0);
    int missing = p - 1;
    for (int q : primes) {
        int rem = q % p;
        if (rem != 0 && first_prime_for_mod[rem] == 0) {
            first_prime_for_mod[rem] = q;
            missing--;
            if (missing == 0) break;
        }
    }

    mpz_class min_s = 0;
    bool found_any = false;

    for (const auto& fact : factorizations) {
        vector<int> perm = fact;
        sort(perm.begin(), perm.end()); // Required for next_permutation
        
        do {
            mpz_class s_val = 1;
            int W = 1;
            
            for (int c : perm) {
                int best_q = -1;
                
                // Algebraic jump sizes
                for (int k = 1; k <= (p - 1) / W; ++k) {
                    if (std::gcd(k, c) == 1) {
                        int req_res = pow_mod(g, W * k, p);
                        int q = first_prime_for_mod[req_res];
                        
                        // Dynamic prime fallback if > 2,000,000
                        if (q == 0) {
                            q = req_res;
                            if (q < 2) q += p;
                            while (!is_prime_dynamic(q)) q += p;
                            first_prime_for_mod[req_res] = q;
                        }
                        
                        if (best_q == -1 || q < best_q) best_q = q;
                    }
                }
                
                // s_val *= best_q ^ (c - 1) using GMP
                mpz_class q_pow;
                mpz_ui_pow_ui(q_pow.get_mpz_t(), best_q, c - 1);
                s_val *= q_pow;
                
                W *= c;
            }
            
            if (!found_any || s_val < min_s) {
                min_s = s_val;
                found_any = true;
            }
            
        } while (next_permutation(perm.begin(), perm.end()));
    }
    return min_s;
}

int main() {
    int N = 100;
    
    cout << "Building Prime Sieve..." << endl;
    build_sieve();
    
    cout << "Loading cache..." << endl;
    map<int, string> cache = load_cache();
    
    cout << "Calculating minimum 's' for primes < " << N << "...\n\n";
    
    auto start_time = chrono::high_resolution_clock::now();

    for (int p : primes) {
        if (p >= N) break;
        
        if (cache.count(p)) {
            string s_str = cache[p];
            if (s_str.length() > 30) {
                cout << p << "\t| " << s_str.substr(0, 5) << "..." 
                     << s_str.substr(s_str.length() - 5) 
                     << " (" << s_str.length() << " digits) [Cached]\n";
            } else {
                cout << p << "\t| " << s_str << " [Cached]\n";
            }
        } else {
            mpz_class s = find_minimum_s(p);
            save_to_cache(p, s);
            
            string s_str = s.get_str();
            if (s_str.length() > 30) {
                cout << p << "\t| " << s_str.substr(0, 5) << "..." 
                     << s_str.substr(s_str.length() - 5) 
                     << " (" << s_str.length() << " digits)\n";
            } else {
                cout << p << "\t| " << s_str << "\n";
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\nExecution time: " << elapsed.count() << " seconds\n";

    return 0;
}