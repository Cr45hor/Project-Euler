#include <iostream>
#include <cmath>
#include <numeric>

int main() {
    long long N = 1053779;
    long long L1 = 2 * N / std::sqrt(3.0);
    long long L2 = 6 * N / std::sqrt(3.0);
    long long MAXG = std::sqrt(L2);
    
    long long cnt = 0;
    
    for (long long g = 1; g <= MAXG; ++g) {
        long long max_m = L2 / g;
        long long g_mod = g % 3;
        long long l1_g = L1 / g;
        long long l2_g = L2 / g;
        
        for (long long m = g + 1; m <= max_m; ++m) {
            if (std::gcd(m, g) == 1) {
                if (m % 3 != g_mod) {
                    cnt += l1_g / m;
                } else {
                    cnt += l2_g / m;
                }
            }
        }
    }
    
    std::cout << cnt << "\n";
    return 0;
}