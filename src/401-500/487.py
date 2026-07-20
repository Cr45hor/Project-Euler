import math
import numpy as np
from typing import List
import sympy as sp

def extend(coef: List[int]) -> List[int]:
    n = len(coef)
    dp = np.zeros((n, n), dtype=int)
    for i in range(n):
        dp[0][i] = coef[i]
    for i in range(1, n):
        for j in range(i, n):
            dp[i][j] = coef[j] * np.sum(dp[i - 1][i - 1:j])
    # poly = np.sum(dp, axis=1)
    # return [1] + poly.tolist()
    poly = np.zeros(n + 1, dtype=int)
    poly[0] = 1
    for i in range(n):
        poly[i + 1] = np.sum(dp[i][i:n]) * (-1 if i % 2 == 0 else 1)
    return poly.tolist()

def factorial(n: int) -> int:
    res = 1
    for i in range(2, n + 1):
        res *= i
    return res

def f(k: int, n: int) -> int:
    res = 0
    if n <= k + 1:
        res = sum(x**k for x in range(1, n + 1))
    else:
        x = np.arange(0, k + 2)
        y = [sum(x**k for x in range(1, n + 1)) for n in range(0, k + 2)]
        # print(x, y)

        L = np.zeros(k + 2, dtype=np.float64)
        for l in range(0, k + 2):
            y_i = y[l]
            coef = x[:l].tolist() + x[l + 1:].tolist()
            poly = extend(coef)
            denom = factorial(l) * factorial(k + 1 - l) * (-1 if (k + 1 - l) % 2 else 1)
            # print(l, y_i, poly, denom, y_i * np.array(poly) / denom)
            # print(y_i * np.array(poly) / denom)
            L += y_i * np.array(poly) / denom
        # print(k, n, L)

        # Cast L to Numpy polynomial and evaluate at n
        L_reversed = L[::-1]  # Reverse the coefficients for np.polynomial.Polynomial
        poly = np.polynomial.Polynomial(L_reversed)
        res = round(poly(n))

    return res


def G(k: int, N: int) -> int:
    x = np.arange(0, k + 2)
    y = [sum(x**k for x in range(1, n + 1)) for n in range(0, k + 2)]

    L = np.zeros(k + 2, dtype=np.float64)
    for l in range(0, k + 2):
        y_i = y[l]
        coef = x[:l].tolist() + x[l + 1:].tolist()
        poly = extend(coef)
        denom = factorial(l) * factorial(k + 1 - l) * (-1 if (k + 1 - l) % 2 else 1)
        L += y_i * np.array(poly) / denom

    L_reversed = L[::-1]
    # print(L_reversed)
    poly = np.polynomial.Polynomial(L_reversed)

    if N <= k + 1:
        return sum(y[:N + 1])

    res = sum(y)
    for n in range(k + 2, N + 1):
        res += round(poly(n))
    return res

def extend_mod(coef: List[int], p: int) -> List[int]:
    n = len(coef)
    dp = [[0] * n for _ in range(n)]
    sum_dp = [[0] * n for _ in range(n)]

    for i in range(n):
        dp[0][i] = coef[i] % p
        sum_dp[0][i] = sum(dp[0][0:i + 1]) % p
        
    for i in range(1, n):
        for j in range(i, n):
            dp[i][j] = (coef[j] * sum_dp[i - 1][j - 1]) % p
            if j > i:
                sum_dp[i][j] = (sum_dp[i][j - 1] + dp[i][j]) % p
            else:
                sum_dp[i][j] = dp[i][j] % p

    poly = [0] * (n + 1)
    poly[0] = 1
    for i in range(n):
        poly[i + 1] = sum_dp[i][n - 1] % p
        if i % 2 == 0:
            poly[i + 1] = (p - poly[i + 1]) % p
    return poly

def F_mod(k: int, n: int, p: int) -> int:
    # Base case for small n
    if n <= k + 1:
        return sum(pow(x, k, p) for x in range(1, n + 1)) % p

    x = list(range(0, k + 2))
    y = [0] * (k + 2)
    for i in range(1, k + 2):
        y[i] = (y[i - 1] + pow(i, k, p)) % p

    # Precompute factorials and their modular inverses to handle division
    fact = [1] * (k + 2)
    inv_fact = [1] * (k + 2)
    for i in range(2, k + 2):
        fact[i] = (fact[i - 1] * i) % p
    inv_fact[k + 1] = pow(fact[k + 1], -1, p)
    for i in range(k, 0, -1):
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % p
    
    L = [0] * (k + 2)
    for l in range(0, k + 2):
        y_i = y[l]
        coef = x[:l] + x[l + 1:]
        
        # Uses the pure Python extend_mod to avoid Numpy 64-bit overflows
        poly = extend_mod(coef, p)
        
        scaled = (inv_fact[l] * inv_fact[k + 1 - l]) % p
        if (k + 1 - l) % 2 != 0:
            scaled = (p - scaled) % p
            
        multiplier = (y_i * scaled) % p
        
        # Distribute into the final polynomial coefficients L
        for i in range(k + 2):
            L[i] = (L[i] + multiplier * poly[i]) % p

    # Evaluate the polynomial L at n modulo p using Horner's method
    res = 0
    n_mod = n % p
    for i in range(k + 2):
        res = (res * n_mod + L[i]) % p
        
    return res

def G_mod(k: int, N: int, p: int) -> int:
    x = list(range(0, k + 2))
    y = [0] * (k + 2)
    for n in range(1, k + 2):
        y[n] = (y[n - 1] + pow(n, k, p)) % p

    fact = [1] * (k + 2)
    inv_fact = [1] * (k + 2)
    for i in range(2, k + 2):
        fact[i] = (fact[i - 1] * i) % p
    inv_fact[k + 1] = pow(fact[k + 1], -1, p)
    for i in range(k, 0, -1):
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % p
    
    L = [0] * (k + 2)
    for l in range(0, k + 2):
        y_i = y[l]
        coef = x[:l] + x[l + 1:]
        poly = extend_mod(coef, p)
        
        scaled = (inv_fact[l] * inv_fact[k + 1 - l]) % p
        if (k + 1 - l) % 2 != 0:
            scaled = (p - scaled) % p
            
        multiplier = (y_i * scaled) % p
        # Distribute into L using pure Python to avoid 64-bit overflow
        for i in range(k + 2):
            L[i] = (L[i] + multiplier * poly[i]) % p
            
    # # L is ordered highest degree first, Horner's method handles this directly
    # def eval_poly(val: int) -> int:
    #     res = 0
    #     for i in range(k + 2):
    #         res = (res * val + L[i]) % p
    #     return res

    if N <= k + 1:
        return sum(y[:N + 1]) % p
        
    res = sum(y) % p
    # print(x, y)
    # print("Initial sum for n=0 to k+1:", res)
    # for n in range(k + 2, N + 1):
    #     res = (res + eval_poly(n)) % p

    # print(L)
    # assert L[0] == pow(3, -1, p)
    # assert L[1] == pow(2, -1, p)
    # assert L[2] == pow(6, -1, p)
    L = L[::-1]
    for l, c_l in enumerate(L):
        # c_l * (sum of n^l for n = k + 2 to N) = c_l * (F_mod(l, N, p) - F_mod(l, k + 1, p))
        # print(l, c_l, k + 2, N, F_mod(l, N, p), F_mod(l, k + 1, p))
        sum_power = (F_mod(l, N, p) - F_mod(l, k + 1, p)) % p if l > 0 else (N - (k + 1)) % p
        if sum_power < 0:
            sum_power += p
        # print(l, c_l, sum_power)
        res = (res + c_l * sum_power) % p

    return res

if __name__ == "__main__":
    k = 10000
    N = 10**12
    L = 2 * 10**9
    R = 2 * 10**9 + 2000
    p = sp.nextprime(L)
    res = 0

    while p <= R:
        res += G_mod(k, N, p)
        p = sp.nextprime(p)
    print(res)
    # k = 2
    # n = 5
    # # print(G_mod(k, n, 10**9 + 7))
    # for n in range(1, 20):
    #     print(n, G(k, n), G_mod(k, n, 10**9 + 7))
    # k = 4
    # N = 100
    # MOD = 10**9 + 7
    # assert G(k, N) % MOD == G_mod(k, N, MOD)
    # print(G_mod(k, N, MOD))