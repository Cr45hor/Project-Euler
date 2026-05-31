#include <iostream>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

int prev_cnt_moves[3][3];
int cnt_moves[3][3];

pair<int, int> move_mapping(int a, int b, int ra, int rb, int rc) {
    assert(a != b && 0 <= a && a < 3 && 0 <= b && b < 3);
    assert(ra >= 0 && rb >= 0 && rc >= 0);
    assert(ra < 3 && rb < 3 && rc < 3);
    assert(ra != rb && ra != rc && rb != rc);

    int arr_rods[3] = {ra, rb, rc};
    int ma = find(arr_rods, arr_rods + 3, a) - arr_rods;
    int mb = find(arr_rods, arr_rods + 3, b) - arr_rods;
    return {ma, mb};
}

vector<pair<int, int>> move_seq;
int lst = -1;
int hanoi(int n, int a, int b, int c) {
    assert(n > 0);
    assert(a != b && a != c && b != c);
    assert(0 <= a && a < 3 && 0 <= b && b < 3 && 0 <= c && c < 3);

    if (n == 1) {
        if (lst != -1) {
            assert(0 <= lst && lst < 3);
            cnt_moves[lst][a]++;
        }
        cnt_moves[a][c]++;
        move_seq.emplace_back(a, c);
        lst = c;
        return 1;
    }

    int first_half = hanoi(n - 1, a, c, b);

    if (lst != -1) {
        assert(0 <= lst && lst < 3);
        cnt_moves[lst][a]++;
    }
    cnt_moves[a][c]++;
    move_seq.emplace_back(a, c);
    lst = c;

    int second_half = hanoi(n - 1, b, a, c);
    return first_half + 1 + second_half;
}

int step(int n, int a, int b) {
    assert(a != b);
    assert(1 <= a && a <= n);
    assert(1 <= b && b <= n);

    if (a < b) {
        return (b - a) * (a + b - 2); 
    }
    else {
        return (a - b) * (n * 2 - a - b);
    }
}

int main() {
    for (int n = 1; n <= 12; n++) {
        fill(&cnt_moves[0][0], &cnt_moves[0][0] + 3 * 3, 0);
        lst = -1;
        
        int moves = hanoi(n, 0, 1, 2);
        assert(moves == (1 << n) - 1);
        cout << n << ":\n";
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                cout << "M(" << i << "->" << j << ") = " << cnt_moves[i][j] << ", M(" << j << "->" << i << ") = " << cnt_moves[j][i] << '\n';
            }
        }
        cout << '\n';

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                prev_cnt_moves[i][j] = cnt_moves[i][j];
            }
        }
    }
}