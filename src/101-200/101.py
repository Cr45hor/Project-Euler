import numpy as np
from numpy.polynomial.polynomial import Polynomial
from scipy.interpolate import BarycentricInterpolator

def u(n: int) -> int:
    f = Polynomial([1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1])
    return round(f(n))

def OP(k: int, n: int) -> int:
    x = np.arange(1, k + 1)
    y = np.array([u(i) for i in x])

    if k == 1:
        return y[0]
    poly = BarycentricInterpolator(x, y)
    return round(float(poly(n)))

sum = 0
for n in range(1, 10 + 2):
    fit = OP(n, n + 1)
    if u(n + 1) != fit:
        sum += fit
print(sum)