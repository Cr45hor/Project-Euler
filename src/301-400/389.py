import numpy as np
from functools import reduce

def dice(n: int):
    coef = [0] + [1.0 / n] * n
    return np.polynomial.Polynomial(coef)

sides = [4, 6, 8, 12, 20]
distributions = [dice(n) for n in sides]
pgf = reduce(lambda f, g: f(g), distributions)

mean = pgf.deriv(m=1)(1)
variance = pgf.deriv(m=2)(1) + mean - mean**2
print(f"{mean:.4f}")
print(f"{variance:.4f}")