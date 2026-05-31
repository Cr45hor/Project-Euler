#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

int const N = 1e7;

bool is_prime[N];
vector<int> primes;

void sieve() {
    for (int i = 2; i < N; i++) {
        is_prime[i] = true;
    }

    for (int i = 2; i * i < N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes.emplace_back(i);
        }
    }
}

int M(int p, int q) {
    int pn = 1, qm = 1;
    while(1LL * qm * q <= N) {
        qm *= q;
    }
    if (qm == 1) {
        return 0;
    }

    int ans = 0;
    while(1LL * pn * p <= N) {
        pn *= p;
        while(1LL * pn * qm > N) {
            qm /= q;
            if (qm == 1) {
                break;
            }
        }
        if (1LL * pn * qm > N) {
            break;
        }
        if (pn > 1 && qm > 1) {
            ans = max(ans, pn * qm);
        }
    }
    return ans;
}

int main() {
    sieve();

    int64_t sum = 0;
    for (int i = 0; i < (int)primes.size(); i++) {
        int p = primes[i];
        for (int j = i + 1; j < (int)primes.size(); j++) {
            int q = primes[j];
            if (1LL * p * q > N) {
                break;
            }
            sum += M(p, q);
        }
    }
    cout << sum << '\n';
    return 0;
}