#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

const int MOD = 1003443221;

// Pass dp and pair_ind by reference to avoid copying
long long solve(int l, int r, const vector<int>& pair_ind, vector<vector<int>>& dp) {
    if (l > r) return 1;
    
    // Return memoized result if available
    if (dp[l][r] != -1) return dp[l][r];

    long long res = 0;
    int p = pair_ind[l];

    // Case 1: Match l with pair_ind[l]
    if (l < p && p <= r) {
        // Notice the shift: l + 1 instead of l
        long long left_ways = solve(l + 1, p - 1, pair_ind, dp);
        long long right_ways = solve(p + 1, r, pair_ind, dp);
        res = (res + (left_ways * right_ways) % MOD) % MOD;
    }

    // Case 2: Skip l
    res = (res + solve(l + 1, r, pair_ind, dp)) % MOD;

    return dp[l][r] = res;
}

int main() {
    ifstream fin("1001_input.txt");
    if (!fin.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string line;
    if (!getline(fin, line)) return 0;

    vector<int> arr;
    stringstream ss(line);
    string item;
    
    // Parse the comma-separated integers
    while (getline(ss, item, ',')) {
        if (!item.empty()) {
            arr.push_back(stoi(item));
        }
    }

    int S = arr.size();
    cout << S << endl;

    // Use a flat array for pair indices for O(1) lookups
    vector<int> pair_ind(S, -1);
    unordered_map<int, int> match;

    for (int i = 0; i < S; ++i) {
        int v = arr[i];
        if (match.find(v) == match.end()) {
            match[v] = i;
        } else {
            int j = match[v];
            pair_ind[i] = j;
            pair_ind[j] = i;
        }
    }

    // 2D vector for DP initialized to -1
    vector<vector<int>> dp(S, vector<int>(S, -1));

    cout << solve(0, S - 1, pair_ind, dp) << endl;

    return 0;
}