#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <cstdint>
#include <numeric>

using namespace std;

int const N = 16;

int A[N]{};
int n;

int dp[1 << N]{};
int dp_sorted[1 << N]{};
int max_submask[1 << N][N];
int min_submask[1 << N][N];

bool check() {
    sort(A, A + n);
    for (int i = 0; i + 1 < n; i++) {
        if (A[i] == A[i + 1]) return false;
    }

    dp[0] = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        int msb = 31 - __builtin_clz(mask);
        dp[mask] = dp[mask ^ (1 << msb)] + A[msb];
        dp_sorted[mask] = dp[mask];
    }

    // Condition 1.
    sort(dp_sorted, dp_sorted + (1 << n));
    for (int i = 1; i + 1 < (1 << n); i++) {
        if (dp_sorted[i] == dp_sorted[i + 1]) {
            return false;
        }
    }

    long long sum_smallest = A[0];
    long long sum_largest = 0;
    
    for (int k = 1; 2 * k + 1 <= n; k++) {
        sum_smallest += A[k];
        sum_largest += A[n - k];
        
        if (sum_largest >= sum_smallest) {
            return false;
        }
    }

    return true;
}

int main() {
    ifstream fin("0105_sets.txt");
    string line;
    assert(fin.is_open());

    int64_t sum = 0;
    while(getline(fin, line)) {
        n = 0;

        for (int i = 0, j = 0; i < (int)line.size(); i = j) {
            while(j < (int)line.size() && line[j] != ',') {
                j++;
            }
            A[n++] = stoi(line.substr(i, j - i));
            if (j + 1 < (int)line.size()) {
                j++;
            }
        }

        if (check()) {
            sum += accumulate(A, A + n, 0);
        }
    }
    cout << sum << '\n';
}