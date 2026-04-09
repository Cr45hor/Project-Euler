#include <iostream>

using namespace std;

int const N = 1001;
int const dx[] = {1, 0, -1, 0};
int const dy[] = {0, -1, 0, 1};

int grid[N][N];

int main() {
    int mid = N / 2;
    int num = 1;
    int x = mid, y = mid;

    grid[mid][mid] = 1;
    for (int layer = 1; layer <= mid; layer++) {
        y++;
        grid[x][y] = ++num;

        int step = layer * 2 - 1;
        for (int d = 0; d < 4; d++) {
            for (int s = 0; s < step; s++) {
                x += dx[d];
                y += dy[d];
                grid[x][y] = ++num;
            }
            step = layer * 2;
        }
    }

    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += grid[i][i] + grid[i][N - 1 - i];
    }
    cout << sum - 1 << endl; // Subtract the center value which is counted twice
}