import math

limit = 1e12

x, y, n = 1, 1, 1
while n <= limit:
    next_x = x * 3 + y * 4
    next_y = x * 2 + y * 3
    x, y = next_x, next_y
    n = (x + 1) // 2

N = n * n + (n - 1) * (n - 1)
delta = int(math.sqrt(N))
a = (1 + delta) // 2
b = n - a
print(f"a = {a}, b = {b}")