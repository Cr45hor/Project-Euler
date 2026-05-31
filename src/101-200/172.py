# Define F(x) = 1 + x + x^2/2 + x^3/6 is an exponential generating function of the sequence 1, 1, 1, 1
# And G(x) = 1 + x + x^2/2 is an exponential generating function of the sequence 1, 1, 1
# Calculate [x^18/18!]F(x)^10 - [x^17/17!](F(x)^9 * G(x))

from sympy import symbols, series, factorial

x = symbols('x')
F = 1 + x + x**2 / 2 + x**3 / 6
G = 1 + x + x**2 / 2

p1 = series(F**10, x, 0, 19).removeO()
p2 = series(F**9 * G, x, 0, 18).removeO()
result = p1.coeff(x, 18) * factorial(18) - p2.coeff(x, 17) * factorial(17)
print(result)