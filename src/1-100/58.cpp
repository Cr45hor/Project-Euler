#include <iostream>
#include <cstdint>
#include <gmpxx.h>

using namespace std;

mpz_class NE(int n) {
    return mpz_class(n) * mpz_class(n) * 4 - mpz_class(n) * 10 + 7;
}

mpz_class NW(int n) {
    return (mpz_class(n) - 1) * (mpz_class(n) - 1) * 4 + 1;
}

mpz_class SW(int n) {
    return mpz_class(n) * mpz_class(n) * 4 - mpz_class(n) * 6 + 3;
}

mpz_class SE(int n) {
    return (mpz_class(n) * 2 - 1) * (mpz_class(n) * 2 - 1);
}

mpz_class (*dir[])(int) = {NE, NW, SW, SE};

int main() {
    int total = 1;
    int total_primes = 0;
    for (int n = 2; ; n++) {
        for (int w = 0; w < 4; w++) {
            mpz_class corner = dir[w](n);
            total++;
            if (mpz_probab_prime_p(corner.get_mpz_t(), 25) > 0) total_primes++;
        }

        if ((long double)1.0 * total_primes / total < 0.1) {
            cout << n * 2 - 1 << '\n';
            break;
        }
    }
}