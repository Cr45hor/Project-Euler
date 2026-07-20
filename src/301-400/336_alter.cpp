#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int N = 11;
    // Base case for n = 2
    vector<vector<int>> minimax_perms = {{1, 0}}; 

    for (int n = 3; n <= N; n++) {
        vector<vector<int>> next_perms;
        
        for (const auto& P : minimax_perms) {
            // 1. Shift elements up by 1 and prepend 0
            vector<int> base_P(n);
            base_P[0] = 0;
            for (int i = 0; i < n - 1; i++) {
                base_P[i + 1] = P[i] + 1;
            }

            // 2. Reverse the entire array
            reverse(base_P.begin(), base_P.end());

            // 3. Apply the second reverse for every valid 'curr'
            for (int curr = 1; curr < n - 1; curr++) {
                vector<int> new_perm = base_P;
                reverse(new_perm.begin() + curr, new_perm.end());
                next_perms.push_back(new_perm);
            }
        }
        minimax_perms = next_perms;
    }

    // Sort lexicographically to find the specific arrangement
    sort(minimax_perms.begin(), minimax_perms.end());

    // Print the 2011th permutation
    if (minimax_perms.size() >= 2011) {
        for (int x : minimax_perms[2011 - 1]) {
            cout << (char)(x + 'A');
        }
        cout << "\n";
    }

    return 0;
}