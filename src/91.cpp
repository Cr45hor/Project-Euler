#include <iostream>

using namespace std;

int const N = 50;

bool check(int xA, int yA, int xB, int yB, int xC, int yC) {
    int xBA = xA - xB;
    int yBA = yA - yB;
    int xBC = xC - xB;
    int yBC = yC - yB;
    return xBA * xBC + yBA * yBC == 0;
}

int main() {
    int cnt = 0;
    for (int x1 = 0; x1 <= N; x1++) {
        for (int y1 = 0; y1 <= N; y1++) {
            if (x1 == 0 && y1 == 0) {
                continue;
            }
            for (int x2 = 0; x2 <= N; x2++) {
                for (int y2 = 0; y2 <= N; y2++) {
                    if (x2 == 0 && y2 == 0) {
                        continue;
                    }
                    if (x2 == x1 && y2 == y1) {
                        continue;
                    }

                    bool checkO = check(x1, y1, 0, 0, x2, y2);
                    bool checkP = check(0, 0, x1, y1, x2, y2);
                    bool checkQ = check(0, 0, x2, y2, x1, y1);
                    if (checkO || checkP || checkQ) {
                        cnt++;
                        // cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';
                    }
                }
            }
        }
    }
    cout << cnt/2;
}