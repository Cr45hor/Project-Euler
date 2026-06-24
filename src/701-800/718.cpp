#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Helper to compute exponents
int64_t power(int64_t base, int exp) {
    int64_t res = 1;
    for (int i = 0; i < exp; i++) res *= base;
    return res;
}

// Extended Euclidean Algorithm
int64_t extGCD(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int64_t x1, y1;
    int64_t d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// Modular inverse
int64_t modInverse(int64_t n, int64_t m) {
    int64_t x, y;
    extGCD(n, m, x, y);
    return (x % m + m) % m;
}

// Helper to print 128-bit integers
void print128(__int128_t n) {
    if (n == 0) {
        cout << 0 << '\n';
        return;
    }
    string s;
    while (n > 0) {
        s += (char)('0' + (n % 10));
        n /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s << '\n';
}

int const MOD = 1e9 + 7;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int64_t A = power(17, 6);
    int64_t B = power(19, 6);
    int64_t C = power(23, 6);

    int64_t MIN_DIS = A + B + C;
    
    // We immediately cast to 128-bit to avoid overflow
    __int128_t res_128 = (__int128_t)MIN_DIS * (MIN_DIS - 1) / 2;

    // Step 1: Pre-calculate the pure +C step costs
    int64_t invC = modInverse(C % A, A);
    int64_t K = (B % A) * invC % A;

    // D[i] stores the minimum cost to reach residue (i * B) % A
    vector<int64_t> D(A);
    for (int i = 0; i < A; i++) {
        int64_t v = (1LL * i * K) % A;
        D[i] = v * C;
    }

    // Step 2: Linear DP to propagate +B steps (No Dijkstra needed)
    // Going around the cycle twice guarantees all optimal +B sequences are found
    for (int step = 0; step < 2 * A; step++) {
        int i = step >= A ? step - A : step;
        int nxt = i + 1 == A ? 0 : i + 1;
        if (D[i] + B < D[nxt]) {
            D[nxt] = D[i] + B;
        }
    }

    // Step 3: Mathematical equivalent of the 10^15 delta loop
    for (int i = 0; i < A; i++) {
        int64_t r = (1LL * i * (B % A)) % A;
        int64_t c_r = (D[i] - r) / A;
        
        if (c_r > 0) {
            __int128_t cr = c_r;
            // Sum of arithmetic progression instead of stepping 1-by-1
            res_128 += cr * (r + MIN_DIS) + (__int128_t)A * cr * (cr - 1) / 2;
        }
    }

    print128((res_128 % MOD));

    return 0;
}