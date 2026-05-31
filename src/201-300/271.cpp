#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

using u64 = long long;
using u128 = __uint128_t;

u64 binpower(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1)
            result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

long tonelli_shanks(long n, long p) {
    long s = 0;
    long q = p - 1;
    while ((q & 1) == 0) { q /= 2; ++s; }
    if (s == 1) {
        long r = binpower(n, (p+1)/4, p);
        if ((r * r) % p == n) return r;
        return 0;
    }
    // Find the first quadratic non-residue z by brute-force search
    long z = 1;
    while (binpower(++z, (p-1)/2, p) != p - 1);
    long c = binpower(z, q, p);
    long r = binpower(n, (q+1)/2, p);
    long t = binpower(n, q, p);
    long m = s;
    while (t != 1) {
        long tt = t;
        long i = 0;
        while (tt != 1) {
            tt = (tt * tt) % p;
            ++i;
            if (i == m) return 0;
        }
        long b = binpower(c, binpower(2, m-i-1, p-1), p);
        long b2 = (b * b) % p;
        r = (r * b) % p;
        t = (t * b2) % p;
        c = b2;
        m = i;
    }
    if ((r * r) % p == n) return r;
    return 0;
}

int normalize(long long x, int p) {
    x %= p;
    if (x < 0) {
        x += p;
    }
    return x;
}

tuple<int, int, int> solution(int p) {
    assert(p % 3 == 1);
    int sqrt_minus_3 = tonelli_shanks(p - 3, p);
    if (!(sqrt_minus_3 & 1)) {
        sqrt_minus_3 = p - sqrt_minus_3;
    }
    int x1 = (-1 + sqrt_minus_3) / 2;
    int x2 = (-1 - sqrt_minus_3) / 2;
    return make_tuple(1, normalize(x1, p), normalize(x2, p));
}

// int const primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43};
int const primes[] = {7, 13};

long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    
    if (x1 < 0) x1 += m0;
    return x1;
}

long long crt(vector<pair<long long, long long>> const& congruences) {
    long long prod = 1;
    for (auto [_, mod] : congruences) {
        prod *= mod;
    }

    long long result = 0;
    for (auto [rem, mod] : congruences) {
        long long pp = prod / mod;
        
        // Pass pp % mod to keep the starting numbers small in the EEA
        long long inv = modInverse(pp % mod, mod); 
        
        // Use __int128_t to safely absorb the massive ~10^24 intermediate multiplication
        __int128_t term = (__int128_t)rem * pp;
        term = (term * inv) % prod;
        
        // term is now safely modulo'd back down to < 1.3 * 10^16, safe to cast back
        result = (result + (long long)term) % prod;
    }
    
    return (result + prod) % prod; 
}

int main() {
    vector<pair<tuple<int, int, int>, int>> solutions;
    long long prod = 1;
    for (int p : primes) {
        if (p % 3 == 1) {
            auto [a, b, c] = solution(p);
            solutions.emplace_back(make_tuple(a, b, c), p);
            cout << "Prime: " << p << " Solutions: " << a << ", " << b << ", " << c << "\n";
        }
        else {
            prod *= p;
        }
    }
    cout << "Product of primes congruent to 2 mod 3 and 3: " << prod << "\n";

    // Iterate all possible remainders for each prime in "solutions", then add "x = 1 mod prod" to the list of congruences and solve using CRT.

    // Vector to store the final combined results
    vector<long long> final_answers;

    // The starting system of congruences contains the base condition
    vector<pair<long long, long long>> current_congruences;
    current_congruences.push_back({1, prod}); // x = 1 mod prod

    // Recursive lambda to generate all combinations
    auto solve_combinations = [&](auto& self, int idx) -> void {
        // Base case: We have selected a remainder for every prime in 'solutions'
        if (idx == (int)solutions.size()) {
            // Solve the current system using your CRT function
            final_answers.push_back(crt(current_congruences));
            return;
        }

        // Extract the tuple of remainders and the prime modulo
        auto [rem_tuple, p] = solutions[idx];
        auto [a, b, c] = rem_tuple;

        // Iterate through the 3 possible remainders for this prime
        long long choices[] = {a, b, c};
        for (long long rem : choices) {
            // 1. Add the choice to the system
            current_congruences.push_back({rem, p});
            
            // 2. Recurse to the next prime
            self(self, idx + 1);
            
            // 3. Backtrack (remove the choice to try the next one)
            current_congruences.pop_back();
        }
    };

    // Kick off the recursion starting at index 0
    solve_combinations(solve_combinations, 0);

    // Print all discovered solutions
    cout << "Total solutions found: " << final_answers.size() << "\n";
    for (long long ans : final_answers) {
        cout << ans << "\n";
    }
}