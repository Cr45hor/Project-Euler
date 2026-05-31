#include <iostream>
#include <assert.h>

using namespace std;

int const N = 2000;
long long const oo = 1e12;

long long s[N * N + 1];
long long grid[N][N];

void generate_grid() {
    for (int k = 1; k <= N * N; k++) {
        if (k <= 55) {
            s[k] = (100003LL - 200003LL * k + 300007LL * k * k * k) % 1000000;
        } else {
            s[k] = (s[k - 24] + s[k - 55] + 1000000) % 1000000;
        }
        s[k] -= 500000;
    }
    assert(s[10] == -393027);
    assert(s[100] == 86613);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = s[i * N + j + 1];
        }
    }
}

long long diagonal_grid[N << 1][N << 1];
void rotate_45() {
    for (int i = 0; i < N << 1; i++) {
        for (int j = 0; j < N << 1; j++) {
            diagonal_grid[i][j] = -oo;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            diagonal_grid[i + j][N - 1 - i + j] = grid[i][j];
        }
    }
}

void rotate_90() {
    // Transpose
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            swap(grid[i][j], grid[j][i]);
        }
    }

    // Reverse rows
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N / 2; i++) {
            swap(grid[i][j], grid[N - 1 - i][j]);
        }
    }
}

long long ans = -oo;
long long sum[N << 1];

void calc() {
    for (int i = 0; i < N; i++) {
        long long min_sum = 0;
        for (int j = 0; j < N; j++) {
            sum[j + 1] = sum[j] + grid[i][j];
            ans = max(ans, sum[j + 1] - min_sum);
            min_sum = min(min_sum, sum[j + 1]);
        }
    }
    
    for (int i = 0; i + 1 < N << 1; i++) {
        int j = 0;
        while (j < N << 1 && diagonal_grid[i][j] == -oo) {
            j++;
        }
        if (j == N << 1) {
            continue;
        }

        long long min_sum = 0;
        int pre_id = 0;
        sum[0] = 0;
        for (; j < N << 1; j++) {
            if (diagonal_grid[i][j] == -oo) {
                continue;
            }
            pre_id++;
            sum[pre_id] = sum[pre_id - 1] + diagonal_grid[i][j];
            ans = max(ans, sum[pre_id] - min_sum);
            min_sum = min(min_sum, sum[pre_id]);
        }
    }
}

int main() {
    generate_grid();
    
    for (int i = 0; i < 2; i++) {
        rotate_45();
        calc();
        rotate_90();
    }

    cout << ans << endl;
    return 0;
}