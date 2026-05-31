#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <string>

using namespace std;

const vector<int> PRIMES = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
};

bool is_p_smooth(mpz_class val) {
    if (val <= 0) return false;
    for (int p : PRIMES) {
        while (val % p == 0) {
            val /= p;
        }
    }
    return val == 1;
}

// Returns both X1 and Y1
pair<mpz_class, mpz_class> solve_pell_X1_Y1(uint64_t d_in) {
    uint64_t a0 = sqrt(d_in);
    if (a0 * a0 == d_in) {
        return {0, 0}; 
    }

    uint64_t m = 0;
    uint64_t d_val = 1;
    uint64_t a = a0;

    mpz_class p_prev(1), p_curr(to_string(a0));
    mpz_class q_prev(0), q_curr(1);
    
    uint64_t M00 = 1, M01 = 0;
    uint64_t M10 = 0, M11 = 1;
    uint64_t k = 0;

    while (true) {
        m = d_val * a - m;
        d_val = (d_in - m * m) / d_val;

        if (d_val == 1 && k % 2 == 1) break; 

        a = (a0 + m) / d_val;
        k++;

        if (M00 > UINT64_MAX / (a + 1)) {
            mpz_class m00_mpz(to_string(M00)), m01_mpz(to_string(M01));
            mpz_class m10_mpz(to_string(M10)), m11_mpz(to_string(M11));

            mpz_class new_p_curr = m00_mpz * p_curr + m01_mpz * p_prev;
            mpz_class new_p_prev = m10_mpz * p_curr + m11_mpz * p_prev;
            p_curr = new_p_curr;
            p_prev = new_p_prev;

            mpz_class new_q_curr = m00_mpz * q_curr + m01_mpz * q_prev;
            mpz_class new_q_prev = m10_mpz * q_curr + m11_mpz * q_prev;
            q_curr = new_q_curr;
            q_prev = new_q_prev;

            M00 = 1; M01 = 0;
            M10 = 0; M11 = 1;
        }

        uint64_t next_M00 = a * M00 + M10;
        uint64_t next_M01 = a * M01 + M11;
        M10 = M00; M11 = M01;
        M00 = next_M00; M01 = next_M01;
    }

    mpz_class m00_mpz(to_string(M00)), m01_mpz(to_string(M01));
    mpz_class final_p_curr = m00_mpz * p_curr + m01_mpz * p_prev;
    mpz_class final_q_curr = m00_mpz * q_curr + m01_mpz * q_prev;
    
    return {final_p_curr, final_q_curr};
}

int main() {
    cout << "Starting rigorous C++/GMP analysis for p = " << PRIMES.back() << "..." << endl;
    
    int k = PRIMES.size();
    uint64_t total_subsets = (1ULL << k);
    vector<uint64_t> square_free_ds;
    
    for (uint64_t mask = 1; mask < total_subsets; ++mask) {
        uint64_t d = 1;
        for (int i = 0; i < k; ++i) {
            if (mask & (1ULL << i)) {
                d *= PRIMES[i];
            }
        }
        square_free_ds.push_back(d);
    }
    
    sort(square_free_ds.begin(), square_free_ds.end());
    int total_ds = square_free_ds.size();

    mpz_class absolute_max_n(0);
    mpz_class total_sum_n(0);
    int valid_pairs_count = 0;

    auto start_time = chrono::high_resolution_clock::now();

    for (int idx = 0; idx < total_ds; ++idx) {
        uint64_t d = square_free_ds[idx];
        mpz_class d_mpz(to_string(d));
        
        auto [X1, Y1] = solve_pell_X1_Y1(d);
        
        // Gatekeeper: If Y1 isn't smooth, no higher Y_k can be. 
        if (Y1 > 0 && is_p_smooth(Y1)) {
            mpz_class X_k = X1;
            mpz_class Y_k = Y1;
            
            // Check the Pell sequence up to k=50
            for (int step = 1; step <= 50; ++step) {
                if (X_k % 2 == 1) {
                    mpz_class n = (X_k - 1) / 2;
                    mpz_class n_plus_1 = n + 1;
                    
                    if (n > 0 && is_p_smooth(n) && is_p_smooth(n_plus_1)) {
                        total_sum_n += n;
                        valid_pairs_count++;
                        
                        if (n > absolute_max_n) absolute_max_n = n;
                    }
                }
                
                // Generate next term in sequence
                mpz_class next_X = X1 * X_k + d_mpz * Y1 * Y_k;
                mpz_class next_Y = X1 * Y_k + Y1 * X_k;
                X_k = next_X;
                Y_k = next_Y;
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    cout << "\nFinished in " << elapsed.count() << " seconds.\n";
    cout << "Total valid pairs found: " << valid_pairs_count << "\n";
    cout << "Total Sum of n: " << total_sum_n.get_str() << "\n";
    cout << "Max n: " << absolute_max_n.get_str() << "\n";

    return 0;
}