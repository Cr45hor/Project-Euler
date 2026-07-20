#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <cassert>

using namespace std;

int const N = 11;

map<vector<int>, int> cost;

int main() {
    vector<int> P(N);
    iota(P.begin(), P.end(), 0);

    cost[P] = 0;
    int biggest_cost = 0;
    while(next_permutation(P.begin(), P.end())) {
        // Find the first mismatched position
        int pos = N;
        for (int i = 0; i < N; ++i) {
            if (P[i] != i) {
                pos = i;
                break;
            }
        }

        // Find the current position of element "c"
        int curr = find(P.begin(), P.end(), pos) - P.begin();

        // Reverse
        vector<int> next_P = P;
        reverse(next_P.begin() + curr, next_P.end());
        reverse(next_P.begin() + pos, next_P.end());
        assert(next_P[pos] == pos);

        // Assign cost
        cost[P] = cost[next_P] + 1 + (curr < N - 1);
        biggest_cost = max(biggest_cost, cost[P]);
    }

    int minimax_cnt = 0;
    for (auto &[perm, c]: cost) {
        if (c == biggest_cost) {
            minimax_cnt++;
            // if (minimax_cnt == 2011) {
            //     for (int x : perm) {
            //         cout << (char)(x + 'A');
            //     }
            //     cout << "\n";
            //     break;
            // }
            for (int x : perm) {
                cout << (char)(x + 'A');
            }
            cout << "\n";
        }
    }
    cout << "Biggest cost: " << biggest_cost << "\n";
}