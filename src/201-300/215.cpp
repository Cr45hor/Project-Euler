#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

int const W = 32;
int const H = 10;

vector<int64_t> bricks[W + 1];
void gen_row() {
    bricks[0].push_back(1);
    for (int width = 2; width <= W; width++) {
        for (int len: {2, 3}) {
            vector<int64_t> &prev = bricks[width - len];
            for (int64_t mask: prev) {
                bricks[width].push_back(mask | (1LL << width));
            }
        }
    }
}

int match[4000][4000]{};
int64_t dp[H + 1][4000]{};
 
int main() {
    gen_row();

    int64_t accepted_mask = (1LL << W) | 1;
    int total = bricks[W].size();
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < total; j++) {
            match[i][j] = ((bricks[W][i] & bricks[W][j]) == accepted_mask);
        }
    }

    for (int i = 0; i < total; i++) {
        dp[1][i] = 1;
    }
    for (int i = 2; i <= H; i++) {
        for (int j = 0; j < total; j++) {
            for (int k = 0; k < total; k++) {
                if (match[j][k]) {
                    dp[i][j] += dp[i - 1][k];
                }
            }
        }
    }

    int64_t ans = 0;
    for (int i = 0; i < total; i++) {
        ans += dp[H][i];
    }
    cout << ans << endl;

    return 0;
}