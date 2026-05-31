#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <execution>

// Use standard double (8 bytes) for the massive array to keep RAM usage ~3.2 GB
struct Pair {
    double val;
    int i, j;
    bool operator<(const Pair& other) const {
        return val < other.val;
    }
};

struct Cand {
    int i, j, k, l;
};

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    int n = 10000;
    int n2 = n * 2;

    std::cout << "Calculating base values..." << std::endl;
    
    // long double gives ~19 digits, easily clearing the 1e-15 requirement
    long double pi = std::acos(-1.0L); 
    long double pi_over_4 = pi / 4.0L;

    std::vector<long double> arr_exact(n2);
    std::vector<double> arr_d(n2);

    for (int k = 0; k < n2; ++k) {
        long double x = (long double)k / (long double)n;
        arr_exact[k] = std::exp(x) - 1.0L - pi_over_4;
        arr_d[k] = static_cast<double>(arr_exact[k]);
    }

    std::cout << "Generating pairs..." << std::endl;
    size_t num_pairs = (size_t)n2 * (n2 - 1) / 2;
    std::vector<Pair> pairs;
    pairs.reserve(num_pairs);

    for (int i = 0; i < n2; ++i) {
        for (int j = i + 1; j < n2; ++j) {
            pairs.push_back({arr_d[i] + arr_d[j], i, j});
        }
    }

    std::cout << "Sorting pairs..." << std::endl;
    std::sort(std::execution::par_unseq, pairs.begin(), pairs.end());

    std::cout << "Executing two-pointer search..." << std::endl;
    int L = 0;
    int R = pairs.size() - 1;
    double window = 1e-10; 
    std::vector<Cand> candidates;

    while (L <= R) {
        double sum = pairs[L].val + pairs[R].val;
        double abs_sum = std::abs(sum);

        if (abs_sum < window) {
            candidates.push_back({pairs[L].i, pairs[L].j, pairs[R].i, pairs[R].j});

            int temp_R = R - 1;
            while (temp_R >= L && std::abs(pairs[L].val + pairs[temp_R].val) < window) {
                candidates.push_back({pairs[L].i, pairs[L].j, pairs[temp_R].i, pairs[temp_R].j});
                temp_R--;
            }
            int temp_L = L + 1;
            while (temp_L <= R && std::abs(pairs[temp_L].val + pairs[R].val) < window) {
                candidates.push_back({pairs[temp_L].i, pairs[temp_L].j, pairs[R].i, pairs[R].j});
                temp_L++;
            }
        }

        if (sum < 0) {
            L++;
        } else {
            R--;
        }
    }

    std::cout << "Evaluating " << candidates.size() << " candidates with long double..." << std::endl;
    long double best_sum = -1.0L;
    Cand best_cand;

    for (const auto& c : candidates) {
        long double exact_sum = arr_exact[c.i] + arr_exact[c.j] + arr_exact[c.k] + arr_exact[c.l];
        long double abs_exact = std::abs(exact_sum);

        if (best_sum == -1.0L || abs_exact < best_sum) {
            best_sum = abs_exact;
            best_cand = c;
        }
    }

    std::vector<int> final_indices = {best_cand.i, best_cand.j, best_cand.k, best_cand.l};
    std::sort(final_indices.begin(), final_indices.end());

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "\nBest sum found: " << std::scientific << std::setprecision(5) << static_cast<double>(best_sum) << std::endl;
    std::cout << "Result: (" 
              << final_indices[0] << ", " 
              << final_indices[1] << ", "
              << final_indices[2] << ", " 
              << final_indices[3] << ")" << std::endl;
    std::cout << "Execution time: " << std::fixed << diff.count() << " seconds\n";

    return 0;
}