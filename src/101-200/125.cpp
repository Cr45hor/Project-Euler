#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <cstdint>

using namespace std;

int const N = 1e8;
int const sqrtN = 1e4;

int64_t S[sqrtN + 1];
bool diff[N + 1];

bool is_palindrome(int64_t x) {
    string s = to_string(x);
    string t = s;
    reverse(t.begin(), t.end());
    return s == t;
}

int main() {
    for (int i = 1; i <= sqrtN; i++) {
        S[i] = S[i - 1] + (int64_t)i * i;
    }

    for (int i = 0; i < sqrtN; i++) {
        for (int j = i + 2; j <= sqrtN; j++) {
            int64_t sum = S[j] - S[i];
            if (sum > N) {
                break;
            }
            diff[sum] = true;
        }
    }

    int64_t ans = 0;
    for (int i = 1; i < N; i++) {
        if (diff[i] && is_palindrome(i)) {
            ans += i;
        }
    }
    cout << ans << '\n';
    return 0;
}