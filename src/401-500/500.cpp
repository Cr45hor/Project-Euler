#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>

#include "Z.hpp"
int MOD = 500500507;

using namespace std;

int const MX = 1e7;

int spf[MX + 1];
int d[MX + 1];
vector<int> primes;

void sieve() {
    for (int i = 2; i <= MX; i++) {
        spf[i] = i;
    }
    for (int i = 2; i * i <= MX; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MX; j += i) {
                spf[j] = i;
            }
        }
    }

    for (int i = 2; i <= MX; i++) {
        if (spf[i] == i) {
            primes.push_back(i);
        }
    }

    d[1] = 1;
    for (int i = 2; i <= MX; i++) {
        int p = spf[i];
        int n = i, k = 0;
        while (n % p == 0) {
            n /= p;
            k++;
        }
        d[i] = d[n] * (k + 1);
    }
}

int const N = 500500;
vector<int> fermi_dirac;

int main() {
    sieve();

    assert(primes.size() > N);
    for (int p: primes) {
        int p_2k = p;
        while(p_2k <= MX) {
            fermi_dirac.push_back(p_2k);
            if (p_2k > MX / p_2k) {
                break;
            }
            p_2k *= p_2k;
        }
    }
    assert(fermi_dirac.size() > N);

    sort(fermi_dirac.begin(), fermi_dirac.end());
    cout << accumulate(fermi_dirac.begin(), fermi_dirac.begin() + N, Z(1), [](Z prod, int x) {
        return prod * x;
    }) << '\n';
    return 0;
}