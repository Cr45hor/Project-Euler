from fractions import Fraction

f = Fraction(2, 1)

N = 1000
res = 0
for k in range(1, N + 1):
    sqrt2_approx = 1/f + 1
    res += 1 if len(str(sqrt2_approx.numerator)) > len(str(sqrt2_approx.denominator)) else 0
    f = 1/f + 2

print(res)