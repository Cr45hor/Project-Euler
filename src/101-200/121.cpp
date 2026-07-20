#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <numeric>
#include <ranges>

using namespace std;

using polynomial = vector<double>;

const double eps = 1e-9;

void remove_leading_zeros(polynomial &p) {
    while (!p.empty() && abs(p.back()) < eps) {
        p.pop_back();
    }
}

void print_polynomial(const polynomial &p) {
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i];
        if (i > 0) {
            cout << "x^" << i;
        }
        if (i + 1 < p.size()) {
            cout << " + ";
        }
    }
    cout << endl;
}

polynomial add(const polynomial &A, const polynomial &B) {
    size_t max_size = max(A.size(), B.size());
    polynomial result(max_size, 0.0);
    for (size_t i = 0; i < max_size; ++i) {
        if (i < A.size()) result[i] += A[i];
        if (i < B.size()) result[i] += B[i];
    }
    remove_leading_zeros(result);
    return result;
}

polynomial multi(const polynomial &A, const polynomial &B) {
    if (A.empty() || B.empty()) return {};
    polynomial result(A.size() + B.size() - 1, 0.0);
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < B.size(); ++j) {
            result[i + j] += A[i] * B[j];
        }
    }
    remove_leading_zeros(result);
    return result;
}

polynomial multi(const polynomial &A, double scalar) {
    polynomial result(A.size(), 0.0);
    for (size_t i = 0; i < A.size(); ++i) {
        result[i] = A[i] * scalar;
    }
    remove_leading_zeros(result);
    return result;
}

double eval(const polynomial &p, double x) {
    double result = 0.0;
    for (double coeff: p | std::views::reverse) {
        result = result * x + coeff;
    }
    return result;
}

// ----------------------------------------------------------------------------

polynomial turn(int r, int b, bool scaled = false) {
    int s = r + b;
    polynomial res = {1.0 * r / s, 1.0 * b / s};
    if (scaled) {
        res = multi(res, 0.5);
    }
    return res;
}

int const N = 15;

int main() {
    polynomial res = {1.0}; 
    for (int k = 1; k <= N; ++k) {
        res = multi(res, turn(k, 1, false));
    }

    double win_probability = 0.0;
    int needed_to_win = (N / 2) + 1;
    
    for (size_t i = needed_to_win; i < res.size(); ++i) {
        win_probability += res[i];
    }

    long long max_prize = 1.0 / win_probability;
    cout << max_prize << '\n';

    return 0;
}