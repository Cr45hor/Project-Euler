#include <iostream>
#include <gmpxx.h>
#include <set>
#include <cstdint>

using namespace std;

const int N = 1e9;

int spf[N];
int prime_index[N];
int cnt_primes = 0;

void sieve() {
    spf[2] = 2;
    for (int i = 4; i < N; i += 2) {
        spf[i] = 2;
    }
    for (int i = 1; i < N; i += 2) {
        spf[i] = i;
    }
    for (int i = 3; i * i < N; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i * 2) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    for (int i = 2; i < N; i++) {
        if (spf[i] == i) {
            prime_index[i] = ++cnt_primes;
        }
    }
}

bool admissible(int n) {
    int buf[16];
    int l = 0;
    while(n > 1) {
        int p = spf[n];
        while(n % p == 0) {
            n /= p;
        }
        buf[l++] = prime_index[p];
    }
    return buf[l - 1] == l;
}

int main() {
    sieve();

    set<int> M;
    for (int n = 2; n < N; n++) {
        if (admissible(n)) {
            mpz_class st = n + 1;
            mpz_class next_prime;
            mpz_nextprime(next_prime.get_mpz_t(), st.get_mpz_t());
            mpz_class gap = next_prime - st;
            int m = (int)mpz_get_ui(gap.get_mpz_t()) + 1;
            M.insert(m);
        }
    }

    int64_t sum = 0;
    for (int m : M) {
        sum += m;
    }
    cout << sum << '\n';
    return 0;
}