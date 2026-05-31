#include <iostream>

using namespace std;

int const N = 12000;

struct farey_sequence {
    int n;
    int index = 0;
    int a = 0, b = 1; 
    int c = 1, d;

    farey_sequence(int order) : n(order), d(order) {}

    pair<int, int> next() {
        if (index == 0) {
            index++;
            return {a, b};
        }
        else if (index == 1) {
            index++;
            return {c, d};
        }

        int k = (n + b) / d;
        int p = k * c - a;
        int q = k * d - b;

        a = c, b = d;
        c = p, d = q;
        index++;
        return {p, q};
    }
};

int main() {
    farey_sequence f(N);
    
    int L = 0, R = 1;
    for (int i = 0; ; i++) {
        auto [a, b] = f.next();
        if (a == 1 && b == 3) {
            L = i;
        }
        else if (a == 1 && b == 2) {
            R = i;
            break;
        }
    }

    cout << R - L - 1 << '\n';
}