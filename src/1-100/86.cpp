#include <iostream>
#include <cmath>

using namespace std;

int const M = 100;

int main() {
    int cnt = 0;

    for (int c = 1; ; c++) {
        for (int s_ab = 2; s_ab <= c * 2; s_ab++) {
            int d2 = s_ab * s_ab + c * c;
            int d = sqrt(d2);
            if (d * d == d2) {
                cnt += s_ab / 2 - (s_ab > c ? s_ab - c - 1 : 0);
            }
        }
        if (cnt > 1000000) {
            cout << c << '\n';
            break;
        }
    }
}