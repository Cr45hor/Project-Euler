#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string num_str(int x, int base) {
    if (x == 0) {
        return "0";
    }
    string res;
    while(x > 0) {
        int d = x % base;
        res.push_back((char)(d + '0'));
        x /= base;
    }
    return res;
}

bool is_palindrone(string s) {
    string tmp = s;
    reverse(tmp.begin(), tmp.end());
    return (tmp == s);
}

int main() {
    int const N = 1000000;
    long long sum = 0;

    for (int x = 1; x <= N; x++) {
        if (is_palindrone(num_str(x, 10)) && is_palindrone(num_str(x, 2))) {
            sum += x;
        }
    }

    cout << sum;
}