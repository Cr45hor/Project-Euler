#include <iostream>
#include <assert.h>
#include <set>

using namespace std;

int const N = 1e6;
int const M = N << 2;

int spf[M];
void sieve() {
    spf[1] = 1;
    for (int x = 2; x < M; x++) {
        if (!spf[x]) {
            spf[x] = x;

            if (1LL * x * x >= M) {
                continue;
            }
            for (int y = x * x; y < M; y += x) {
                if (!spf[y]) {
                    spf[y] = x;
                }
            }
        }
    }
}

int prime_factor[32];
int prime_power[32];
int num_prime_factor = 0;
void factorize(int n) {
    assert(n > 1);

    num_prime_factor = 0;
    while (n > 1) {
        int p = spf[n];
        int k = 0;
        while(n % p == 0) {
            k++;
            n /= p;
        }

        prime_factor[num_prime_factor] = p;
        prime_power[num_prime_factor] = k;
        num_prime_factor++;
    }
}

int divisor[1344];
int num_divisor = 0;
void list_divisor(int n) {
    factorize(n);

    divisor[0] = 1;
    num_divisor = 1;

    for (int i = 0; i < num_prime_factor; i++) {
        int pk = 1;
        int p = prime_factor[i];
        int k = prime_power[i];
        int saved_num_divisor = num_divisor;
        for (int d = 1; d <= k; d++) {
            pk *= p;
            for (int j = 0; j < saved_num_divisor; j++) {
                divisor[num_divisor++] = divisor[j] * pk;
            }
        }
    }
}

int count_solution(int n) {
    list_divisor(n << 2);

    set<pair<int, int>> solution;
    for (int i = 0; i < num_divisor; i++) {
        int p = divisor[i];
        int q = (n << 2) / p;
        if (p > q || (p & 1) != (q & 1) || (p + q) % 8 || (q - p) % 4) {
            continue;
        }

        int d = (p + q) / 8, m = (q - p) / 2;
        int x1 = d * 2 - m / 2;
        int x2 = d * 2 + m / 2;

        if (x1 - d > 0) {
            solution.insert({d, x1});
        }
        solution.insert({d, x2});
    }

    return solution.size();
}

int main() {
    sieve();

    int result = 0;

    for (int n = 1; n < N; n++) {
        if (count_solution(n) == 10) {
            result++;
        }
    }

    cout << result;
}