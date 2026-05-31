#include <iostream>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "../Z.hpp"
int MOD = 999999937;

using namespace std;

int64_t r = 1;
template<typename T>
struct Q_adj {
    // a + b * sqrt(r)
    T a = 0, b = 0;
    Q_adj() {}
    Q_adj(T init_a, T init_b) : a(init_a), b(init_b) {}

    Q_adj adj() const {
        return Q_adj(a, -b);
    }
    Q_adj operator-() const {
        return Q_adj(-a, -b);
    }
    Q_adj inv() const {
        T norm = a * a - b * b * r;
        return Q_adj(a / norm, -b / norm);
    }

    Q_adj operator+(const Q_adj& other) const {
        return Q_adj(a + other.a, b + other.b);
    }
    Q_adj operator-(const Q_adj& other) const {
        return Q_adj(a - other.a, b - other.b);
    }
    Q_adj operator*(const Q_adj& other) const {
        return Q_adj(a * other.a + b * other.b * r, a * other.b + b * other.a);
    }
    Q_adj operator/(const Q_adj& other) const {
        return *this * other.inv();
    }

    Q_adj& operator+=(const Q_adj& other) {
        return *this = *this + other;
    }
    Q_adj& operator-=(const Q_adj& other) {
        return *this = *this - other;
    }
    Q_adj& operator*=(const Q_adj& other) {
        return *this = *this * other;
    }
    Q_adj& operator/=(const Q_adj& other) {
        return *this = *this / other;
    }

    bool operator==(const Q_adj& other) const {
        return a == other.a && b == other.b;
    }

    Q_adj pow(int64_t n) const {
        Q_adj res(1, 0), base = *this;
        for (; n; n >>= 1) {
            if (n & 1) 
                res *= base;
            base *= base;
        }
        return res;
    }
};   

Z f(int64_t a, int64_t n) {
    r = a;
    Q_adj<Z> x(int64_t(ceil(sqrt(a))), 1);
    Q_adj<Z> y = x.pow(n);
    
    // Check if 'a' is a perfect square
    int64_t root = round(sqrt(a));
    bool is_perfect_square = (root * root == a);

    // X^n = 2 * A(n) - Y^n
    // If perfect square, Y = 0, floor is 2A(n).
    // If not perfect square, 0 < Y < 1, floor is 2A(n) - 1.
    Z ans = y.a * 2;
    if (!is_perfect_square) {
        ans = ans - Z(1); 
    }
    
    return ans;
}

int main() {
    Z res = 0;
    for (int a = 1; a <= 5000000; a++) {
        res += f(a, 1LL * a * a);
    }
    cout << res << '\n';
}