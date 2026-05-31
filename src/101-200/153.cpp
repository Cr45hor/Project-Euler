#include <iostream>
#include <numeric>

using namespace std;

int const N = 1e8;

// We ONLY need the t array now!
int t[N + 1]{};

void precompute() {
    for (int a = 1; a * a <= N; a++) {
        int a2 = a * a; // Hoist the multiplication to the outer loop
        
        // If 'a' is even, 'b' MUST be odd. So we increment by 2.
        // If 'a' is odd, 'b' can be anything. We increment by 1.
        int b_step = (a % 2 == 0) ? 2 : 1;
        
        for (int b = 1; a2 + b * b <= N; b += b_step) {
            if (gcd(a, b) == 1) {
                t[a2 + b * b] += a;
            }
        }
    }
}

long long get_total_sum_divisors(long long n) {
    long long total_sum = 0;
    for (long long l = 1, r; l <= n; l = r + 1) {
        long long q = n / l;
        r = n / q;
        long long count = r - l + 1;
        long long sum_d = l + r;
        
        if (count % 2 == 0) count /= 2;
        else sum_d /= 2;
        
        total_sum += q * (count * sum_d);
    }
    return total_sum;
}

int main() {
    precompute();

    // 1. Get the base sum of divisors
    long long res = get_total_sum_divisors(N);
    
    // 2. Compute the U sum dynamically without arrays
    long long u_sum = 0;
    
    for (long long l = 1, r; l <= N; l = r + 1) {
        // M is floor(N / x), which is constant for this whole block [l, r]
        long long M = N / l;
        r = N / M;
        
        // Compute the inner sum F(M) ONCE for this entire block
        long long f_M = get_total_sum_divisors(M);
        
        // Sum up all the t[x] values in this block
        long long sum_t = 0;
        for (int x = l; x <= r; x++) {
            sum_t += t[x];
        }
        
        // Add block contribution to the total U sum
        u_sum += sum_t * f_M;
    }
    
    // Add 2 * U to our result based on your earlier math
    res += u_sum * 2;
    
    cout << res << '\n';
    return 0;
}