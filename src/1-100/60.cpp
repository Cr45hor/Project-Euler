#include <iostream>
#include <gmpxx.h>
#include <bitset>
#include <vector>

using namespace std;

int const N = 1e4;
int const M = 1e4;

bitset<N> is_prime;
vector<int> primes;

void sieve() {
    is_prime.set(); 
    is_prime[0] = is_prime[1] = 0; 

    for (int i = 4; i < N; i += 2) {
        is_prime[i] = 0; 
    }

    for (int i = 3; i * i < N; i += 2) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += 2 * i) {
                is_prime[j] = 0; 
            }
        }
    }

    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
}

long long concat(int x, int y) {
    long long pow = 10;
    while (pow <= y) pow *= 10;
    return (long long)x * pow + y;
}

bool check_prime(long long x) {
    mpz_class n = mpz_class(std::to_string(x));
    return mpz_probab_prime_p(n.get_mpz_t(), 25) > 0;
}

int match[M][M];

int main() {
    sieve();
    int num_primes = (int)primes.size();

    for (int i = 0; i < num_primes; i++) {
        for (int j = 0; j < num_primes; j++) {
            match[i][j] = check_prime(concat(primes[i], primes[j])) && check_prime(concat(primes[j], primes[i]));
        }
    }

    // Find a cluster of size 5
    for (int a = 0; a < num_primes; a++) {
        for (int b = a + 1; b < num_primes; b++) {
            if (!match[a][b]) continue;
            for (int c = b + 1; c < num_primes; c++) {
                if (!match[a][c] || !match[b][c]) continue;
                for (int d = c + 1; d < num_primes; d++) {
                    if (!match[a][d] || !match[b][d] || !match[c][d]) continue;
                    for (int e = d + 1; e < num_primes; e++) {
                        if (!match[a][e] || !match[b][e] || !match[c][e] || !match[d][e]) continue;
                        cout << primes[a] + primes[b] + primes[c] + primes[d] + primes[e] << '\n';
                        return 0;
                    }
                }
            }
        }
    }
}