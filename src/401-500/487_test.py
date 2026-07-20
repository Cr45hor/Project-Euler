import math
import numpy as np
from typing import List

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
    poly = np.polynomial.Polynomial(L_reversed)

    if N <= k + 1:
        return sum(y[:N + 1])

    res = sum(y)
    for n in range(k + 2, N + 1):
        res += round(poly(n))
    return res

assert G(4, 100) == 35375333830