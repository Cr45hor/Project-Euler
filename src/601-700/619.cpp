#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#include "../Z.hpp"
const int MOD = 1e9 + 7;

// A highly optimized dynamic bitset tailored for fast XOR basis insertion
struct FastBitset {
    std::vector<uint64_t> words;

    FastBitset(int num_bits) {
        words.assign((num_bits + 63) / 64, 0);
    }

    void flip(int p) {
        words[p / 64] ^= (1ULL << (p % 64));
    }

    // Finds the index of the highest set bit. Returns -1 if all bits are 0.
    int highest_set_bit() const {
        for (int i = (int)words.size() - 1; i >= 0; --i) {
            if (words[i] != 0) {
                // __builtin_clzll returns the number of leading 0-bits
                return i * 64 + 63 - __builtin_clzll(words[i]);
            }
        }
        return -1;
    }

    void xor_with(const FastBitset& other) {
        for (size_t i = 0; i < words.size(); ++i) {
            words[i] ^= other.words[i];
        }
    }
};

std::vector<int> sieve(int n) {
    std::vector<int> spf(n + 1);
    for (int i = 1; i <= n; i += 2) {
        spf[i] = i;
    }
    for (int i = 2; i <= n; i += 2) {
        spf[i] = 2;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j <= n; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
    return spf;
}

Z C(int L, int R) {
    assert(0 <= L && L <= R);
    if (L > R) return 0;

    std::vector<int> spf = sieve(R);
    
    // Map each prime in the range [2, R] to a unique integer ID
    std::vector<int> prime_id(R + 1, -1);
    int num_primes = 0;
    for (int i = 2; i <= R; ++i) {
        if (spf[i] == i) {
            prime_id[i] = num_primes++;
        }
    }

    // basis[i] will store the basis vector whose highest set bit is 'i'
    std::vector<FastBitset> basis(num_primes, FastBitset(0));
    std::vector<bool> has_basis(num_primes, false);
    int basis_size = 0;

    for (int x = L; x <= R; ++x) {
        FastBitset mask(num_primes);
        int temp = x;
        
        // Factorize and toggle the corresponding prime bits
        while (temp > 1) {
            int p = spf[temp];
            int k = 0;
            while(temp % p == 0) {
                temp /= p;
                k++;
            }
            if (k % 2 == 1) { // Only consider odd powers
                mask.flip(prime_id[p]);
            }
        }

        // Insert mask into the XOR basis
        while (true) {
            int pivot = mask.highest_set_bit();
            if (pivot == -1) {
                break; // Vector was reduced to 0
            }
            if (!has_basis[pivot]) {
                has_basis[pivot] = true;
                basis[pivot] = mask;
                basis_size++;
                break;
            } else {
                mask.xor_with(basis[pivot]);
            }
        }
    }

    int dim_ker = (R - L + 1) - basis_size;
    return Z(2).pow(dim_ker) - 1;
}

int main() {
    // assert(C(40, 55) == 15);
    // assert(C(1000, 1234) == 975523611);
    std::cout << C(1000000, 1234567) << '\n';
    return 0;
}