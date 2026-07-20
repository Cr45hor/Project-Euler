#pragma once
#include <iostream>
#include <cstdint>

int normalize(int x, int p) {
    if (x >= p) {
        x -= p;
    }
    else if (x < 0) {
        x += p;
    }
    return x;
}

int binpow(int a, int64_t n, int p) {
    int res = 1, base = a % p;
    for (; n; n >>= 1) {
        if (n & 1) 
            res = 1LL * res * base % p;
        base = 1LL * base * base % p;
    }
    return res;
}

// MOD should be prime, less than 2^30 and is a constant through the program
// The actual value of MOD will be defined in each problem's .cpp file
extern const int MOD; 

// Modular integer class for arithmetic modulo MOD
struct Z{
    int x;
    Z(int X = 0): x(normalize(X, MOD)) {}
    Z(int64_t X): x(normalize(X % MOD, MOD)) {}

    Z operator-() const {
        return Z(-x);
    }
    Z inv() const {
        // Assume MOD is prime, and only used when MOD is truly a prime number
        // Otherwise, need to implement extended Euclidean algorithm to find the inverse
        return Z(binpow(x, MOD - 2, MOD));
    }

    Z operator+(const Z& other) const {
        return Z(x + other.x);
    }
    Z operator-(const Z& other) const {
        return Z(x - other.x);
    }
    Z operator*(const Z& other) const {
        return Z((int64_t)x * other.x);
    }
    Z operator/(const Z& other) const {
       return *this * other.inv();
    }
    Z pow(int64_t n) const {
        return Z(binpow(x, n, MOD));
    }

    Z& operator+=(const Z& other) {
        return *this = *this + other;
    }
    Z& operator-=(const Z& other) {
        return *this = *this - other;
    }
    Z& operator*=(const Z& other) {
        return *this = *this * other;
    }
    Z& operator/=(const Z& other) {
        return *this = *this / other;
    }

    bool operator==(const Z& other) const {
        return x == other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const Z& z) {
        return os << z.x;
    }
    friend std::istream& operator>>(std::istream& is, Z& z) {
        int64_t x;
        is >> x;
        z = Z(x);
        return is;
    }
};