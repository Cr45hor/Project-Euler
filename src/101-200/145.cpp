#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <cstdint>

using namespace std;

int const N = 1e9;

int rev(int x) {
    int y = 0;
    while (x > 0) {
        y = y * 10 + x % 10;
        x /= 10;
    }
    return y;
}

bool check(int x) {
    while(x > 0) {
        int d = x % 10;
        if (!(d & 1)) {
            return false;
        }
        x /= 10;
    }
    return true;
}

int main() {
    int cnt = 0;
    for (int i = 1; i < N; ++i) {
        if ((i % 10 != 0) && check(i + rev(i))) {
            ++cnt;
        }
    }
    cout << cnt << '\n';
    return 0;

}