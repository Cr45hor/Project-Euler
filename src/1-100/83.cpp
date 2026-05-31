#include <iostream>
#include <queue>
#include <vector>
#include <assert.h>
#include <fstream>
#include <sstream>

using namespace std;

template<typename T>
bool minimize(T &a, const T &b) {
    if (b < a) {
        a = b;
        return true;
    }
    return false;
}

int const N = 80;
int const oo = 0x3f3f3f3f;
int const dx[] = {0, 0, -1, 1};
int const dy[] = {-1, 1, 0, 0};

int grid[N][N];
int dist[N][N];

bool is_valid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

int main() {
    ifstream fin("0083_matrix.txt");
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
        for (int j = 0; j < N; j++) {
            dist[i][j] = oo;
        }
    }

    dist[0][0] = grid[0][0];
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    pq.push({dist[0][0], 0, 0});
    while(!pq.empty()) {
        auto [d, x, y] = pq.top();
        pq.pop();
        if (d != dist[x][y]) continue;

        for (int dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (!is_valid(nx, ny)) { 
                continue;
            }
            int nd = d + grid[nx][ny];
            if (minimize(dist[nx][ny], nd)) {
                pq.push({nd, nx, ny});
            }
        }
    }
    cout << dist[N-1][N-1] << endl;
}