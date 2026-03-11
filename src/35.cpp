#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int MAXN = 1e6;

template<int N>
struct Sieve {
    bool is_prime[N]{};
    constexpr Sieve() : is_prime() {
        is_prime[2] = is_prime[3] = true;
        for (int i = 5; i < N; i += 6) {
            is_prime[i] = true;
        }
        for (int i = 7; i < N; i += 6) {
            is_prime[i] = true;
        }
        int N_sqrt = (int)sqrt(N);
        for (int i = 5; i <= N_sqrt; i += 4) {
            if (is_prime[i]) {
                for (int j = i * i; j < N; j += i) {
                    is_prime[j] = false;
                }
            }
            i += 2;
            if (is_prime[i]) {
                for (int j = i * i; j < N; j += i) {
                    is_prime[j] = false;
                }
            }
        }
    }
};

bool fast_is_prime(int n) {
    static constexpr Sieve<MAXN> s;
    return s.is_prime[n];
}

int shift(int x) {
    if (x < 10) {
        return x;
    }

    int lst_digit = x % 10;
    string str_result = to_string(lst_digit) + to_string(x / 10);
    int result = 0;
    for (char digit: str_result) {
        result = result * 10 + (int)digit - (int)'0';
    }
    return result;
}

int main() {
    int cnt = 0;
    for (int x = 1; x < MAXN; x++) {
        if (!fast_is_prime(x)) {
            continue;
        }

        bool flag = true;
        int y = x;
        for (int k = 1; k <= 7; k++) {
            y = shift(y);
            if (y == x) {
                break;
            }
            if (!fast_is_prime(y)) {
                flag = false;
                break;
            }
        }

        if (flag && x < 100) {
            cout << x << '\n';
        } 
        cnt += flag;
    }

    cout << cnt;
}