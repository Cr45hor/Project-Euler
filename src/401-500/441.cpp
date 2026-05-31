#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

const int N = 1e7;

// Allocate arrays globally to prevent stack overflow for large N
int mu[N + 1];
bool is_prime[N + 1];
double hn[N + 1];
vector<int> primes;

int main() {
    // 1. Linear Sieve to compute the Möbius function in O(N)
    fill(is_prime + 2, is_prime + N + 1, true);
    mu[1] = 1;
    
    for (int i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu[i] = -1; // Prime numbers have exactly 1 distinct prime factor
        }
        for (int p : primes) {
            if (i * p > N) break;
            is_prime[i * p] = false;
            
            if (i % p == 0) {
                mu[i * p] = 0; // Contains a squared prime factor
                break;         // Guarantees O(N) complexity
            } else {
                mu[i * p] = -mu[i]; // Multiplicative property
            }
        }
    }

    // 2. Precompute Harmonic Numbers in O(N)
    hn[0] = 0.0;
    for (int i = 1; i <= N; ++i) {
        hn[i] = hn[i - 1] + 1.0 / i;
    }

    // 3. Compute the target sum s in O(N) using our mathematical bridge
    double s = 0;
    for (int g = 1; g <= N; ++g) {
        if (mu[g] != 0) { // Optimization: skip math if mu[g] is 0
            double term = hn[N / g] / g;
            s += mu[g] * term * term;
        }
    }

    // 4. Final output based on the algebraic reduction (s + N - 3) / 2
    double res = (s + N - 3) / 2.0;

    cout << setprecision(4) << fixed << res << '\n';

    return 0;
}