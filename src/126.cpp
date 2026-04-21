#include <iostream>
#include <cmath>

int const MAX_LAYER = 1e8;
int const MIN_CNT = 1000;

long long count_layer(int n, int k) {
    if (n % 2 != 0) return 0;
    
    long long M = (n / 2) + (k * k) - 1;
    long long count = 0;
    
    long long limit_x = std::sqrt(M / 3.0);
    for (long long x = k; x <= limit_x; ++x) {
        long long N = M + x * x;
        long long start_u = std::max(1LL, 2 * x);
        
        for (long long u = start_u; u * u <= N; ++u) {
            if (N % u == 0) {
                count++;
            }
        }
    }
    
    return count;
}

int main() {
    for (int n = 6; n <= MAX_LAYER; n += 2) {
        int limit_k = std::sqrt(n) / 2;
        int cnt_n = 0;
        for (int k = 1; k <= limit_k; ++k) {
            cnt_n += count_layer(n, k);
            if (cnt_n > MIN_CNT) break; 
        }
        if (cnt_n == MIN_CNT) {
            std::cout << n << '\n';
            break;
        }
    }
    std::cout << "Not found within the limit." << '\n';
    return 0;
}