#include <iostream>
#include <cstring>
#include <assert.h>
#include <algorithm>

using namespace std;

const char init[2][101] = {
    "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679", 
    "8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196"
};

int MAX_INDEX = 0;
long long len[80];

void pre() {
    len[0] = strlen(init[0]);
    len[1] = strlen(init[1]);
    for (int i = 2; i < 80; i++) {
        len[i] = len[i - 1] + len[i - 2];
        if (len[i] > 1e18) {
            MAX_INDEX = i;
            break;
        }
    }
}

char D(long long n) {
    int ind = lower_bound(len, len + MAX_INDEX, n) - len;
    while (ind > 1) {
        if (n <= len[ind - 2]) {
            ind -= 2;
        } else {
            n -= len[ind - 2];
            ind -= 1;
        }
    }
    return init[ind][n - 1];
}

int main() {
    pre();

    char res[19]{};
    long long base = 1;
    for (int i = 0; i < 18; i++) {
        res[i] = D(base * (127 + 19 * i));
        base *= 7;
    }
    reverse(res, res + 18);
    cout << res << '\n';
}