#include <iostream>

using namespace std;

int brute_force_prime_factors_count(int n) {
    int count = 0;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            ++count;
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    if (n > 1) {
        ++count;
    }
    return count;
}

int main() {
    int consecutive_count = 0;
    for (int i = 2; ; ++i) {
        if (brute_force_prime_factors_count(i) == 4) {
            ++consecutive_count;
            if (consecutive_count == 4) {
                cout << (i - 3) << endl; // Output the first number in the sequence
                break;
            }
        } else {
            consecutive_count = 0;
        }
    }
    return 0;
}