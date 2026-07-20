#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// Robust integer square root
long long isqrt(long long n) {
    long long x = std::sqrt(n);
    while (x * x > n) x--;
    while ((x + 1) * (x + 1) <= n) x++;
    return x;
}

int main() {
    long long N = 10000000000000000LL;
    // long long N = 1000000;
    
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> L(163, 0);
    std::vector<long long> P(163, 0);
    
    // Euler's criterion for Legendre symbol (i / 163)
    for (int i = 1; i < 163; ++i) {
        long long k = 1;
        long long base = i;
        for (int p = 81; p > 0; p >>= 1) {
            if (p & 1) k = (k * base) % 163;
            base = (base * base) % 163;
        }
        
        L[i] = (k == 162) ? -1 : k; // Handle -1 mod 163
        P[i] = P[i - 1] + L[i];
    }

    // Now P[162] will strictly evaluate to 0.

    long long K = isqrt(N);
    long long sum1 = 0;
    long long sum2 = 0;

    for (long long i = 1; i <= K; ++i) {
        long long q = N / i;
        sum1 += (long long)L[i % 163] * q;
        sum2 += P[q % 163];
    }

    long long correction = K * P[K % 163];
    long long total_solutions = 2LL * (sum1 + sum2 - correction);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "N = " << N << "\n";
    std::cout << "Total Solutions S(N) = " << total_solutions << "\n";
    std::cout << "Execution Time: " << diff.count() << " seconds\n";

    return 0;
}