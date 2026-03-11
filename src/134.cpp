#include <iostream>
#include <string>
#include <cstring>
#include <assert.h>
#include <climits>
#include <vector>

using namespace std;

int const N = 1e6 + 6;

int power(int a, int n, int r = INT_MAX) {
    int res = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            res = 1LL * res * a % r;
        }
        a = 1LL * a * a % r;
    }
    return res;
}

int inv(int a, int p) {
    return power(a, p - 2, p);
}

bool is_prime[N];
vector<int> list_prime;

void sieve() {
    memset(is_prime, 1, sizeof(is_prime));

    is_prime[0] = is_prime[1] = 0;
    for (int x = 2; x < N; x++) {
        if (is_prime[x]) {
            list_prime.push_back(x);
            if (1LL * x * x >= N) {
                continue;
            }
            for (int y = x * x; y < N; y += x) {
                is_prime[y] = false;
            }
        }
    }
}

int num_digit(int x) {
    assert(x >= 0);
    string x_str = to_string(x);
    return x_str.size();
}

int main() {
    sieve();

    long long result = 0;
    for (int i = 2; i < (int)list_prime.size(); i++) {
        int p1 = list_prime[i];
        if (p1 > (int)1e6) {
            break;
        }
        int p2 = list_prime[i + 1];

        int len_p1 = num_digit(p1);
        int base = power(10, len_p1);
        int x = 1LL * (p2 - p1) * inv(base, p2) % p2;
        long long S = 1LL * x * base + p1;

        result += S;
    }
    cout << result;

    return 0;
}