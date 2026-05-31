#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <cstdint>
#include <numeric>

using namespace std;

int const N = 7;

int A[N]{};

int dp[1 << N]{};
int dp_sorted[1 << N]{};
int max_submask[1 << N][N];
int min_submask[1 << N][N];

bool check() {
    int sorted_A[N];
    copy(A, A + N, sorted_A);
    
    sort(sorted_A, sorted_A + N);
    for (int i = 0; i + 1 < N; i++) {
        if (sorted_A[i] == sorted_A[i + 1]) return false;
    }

    dp[0] = 0;
    for (int mask = 1; mask < (1 << N); mask++) {
        int msb = 31 - __builtin_clz(mask);
        dp[mask] = dp[mask ^ (1 << msb)] + sorted_A[msb]; 
        dp_sorted[mask] = dp[mask];
    }

    // Condition 1.
    sort(dp_sorted, dp_sorted + (1 << N));
    for (int i = 1; i + 1 < (1 << N); i++) {
        if (dp_sorted[i] == dp_sorted[i + 1]) {
            return false;
        }
    }

    // Condition 2.
    long long sum_smallest = sorted_A[0];
    long long sum_largest = 0;
    
    for (int k = 1; 2 * k + 1 <= N; k++) {
        sum_smallest += sorted_A[k];
        sum_largest += sorted_A[N - k];
        
        if (sum_largest >= sum_smallest) {
            return false;
        }
    }

    return true;
}

int init_A[N] = {20, 31, 38, 39, 40, 42, 45};
int best_A[N] = {20, 31, 38, 39, 40, 42, 45};
int max_diff = 5;
int min_sum = accumulate(best_A, best_A + N, 0);

void backtrack(int id) {
    assert(id <= N);

    if (id == N) {
        if (check()) {
            int sum = accumulate(A, A + N, 0);
            if (sum < min_sum) {
                min_sum = sum;
                copy(A, A + N, best_A);
            }
        }
        return;
    }

    for (int x = init_A[id] - max_diff; x <= init_A[id] + max_diff; x++) {
        A[id] = x;
        backtrack(id + 1);
    }
}

int main() {
    backtrack(0); 
    for (int i = 0; i < N; i++) {
        cout << best_A[i] << ' ';
    }
    cout << '\n';
}