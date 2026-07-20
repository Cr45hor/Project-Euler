#include <iostream>
#include <iomanip>
#include <cmath>

double R_reduced(long long n) {
    if (n == 0) {
        return 0.0;
    }
    
    double H_n = 0.0;
    
    // For small n, use exact calculation to prevent precision loss on approximations.
    // For large n, the asymptotic expansion is instantaneous and highly accurate.
    if (n < 1000000) {
        for (long long k = 1; k <= n; ++k) {
            H_n += 1.0 / k;
        }
    } else {
        const double gamma = 0.57721566490153286060651209;
        double dn = (double)n;
        // Asymptotic expansion for Harmonic series
        H_n = std::log(dn) + gamma + (1.0 / (2.0 * dn)) - (1.0 / (12.0 * dn * dn));
    }
    
    return 2.0 * (1.0 + 1.0 / n) * H_n - 3.0;
}

double B_reduced(long long n) {
    if (n == 0) return 0.0;
    
    long long k = 0;
    long long temp = n;
    while (temp > 1) {
        temp >>= 1;
        k++;
    }
    
    long long two_pow_k_plus_1 = 1LL << (k + 1);
    
    return 1.0 + (double)((n + 1) * k + 2 - two_pow_k_plus_1) / n;
}

int main() {
    long long N = 10000000000LL;
    
    double result = R_reduced(N) - B_reduced(N);
    
    std::cout << std::fixed << std::setprecision(8) << result << std::endl;
    
    return 0;
}