#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int const N = 80;

int grid[N + 1][N + 1] = {0};
int dp[N + 1][N + 1] = {0};

int main() {
    ifstream fin("0081_matrix.txt");
    for (int i = 1; i <= N; i++) {
        string line;
        getline(fin, line);
        // Each number in this line is separated by a comma
        stringstream ss(line);
        string num;
        int j = 1;
        while (getline(ss, num, ',')) {
            grid[i][j] = stoi(num);
            j++;
        }
    }

    for (int j = 1; j <= N; j++) {
        dp[1][j] = dp[1][j - 1] + grid[1][j];
    }
    for (int i = 1; i <= N; i++) {
        dp[i][1] = dp[i - 1][1] + grid[i][1];
    }
    for (int i = 2; i <= N; i++) {
        for (int j = 2; j <= N; j++) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }

    cout << dp[N][N] << endl;

    return 0;
}