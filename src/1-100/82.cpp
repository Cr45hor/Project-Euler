#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int N = 80;
int grid[N][N];
int cost[N];

int main() {
    ifstream fin("0082_matrix.txt");

    for (int i = 0; i < N; i++) {
        string line;
        getline(fin, line);
        stringstream ss(line);
        string num;
        for(int j = 0; j < N && getline(ss, num, ','); j++) {
            grid[i][j] = stoi(num);
        }
    }

    for (int i = 0; i < N; i++) {
        cost[i] = grid[i][0];
    }

    for (int c = 1; c < N; c++) {
        for (int r = 0; r < N; r++) {
            cost[r] += grid[r][c];
        }

        for (int r = 1; r < N; r++) {
            cost[r] = min(cost[r], cost[r - 1] + grid[r][c]);
        }

        for (int r = N - 2; r >= 0; r--) {
            cost[r] = min(cost[r], cost[r + 1] + grid[r][c]);
        }
    }

    int min_path = cost[0];
    for (int i = 1; i < N; i++) {
        min_path = min(min_path, cost[i]);
    }

    cout << min_path << '\n';

    return 0;
}