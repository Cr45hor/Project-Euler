from decimal import Decimal, getcontext

getcontext().prec = 50

def f(r, k):
    return (Decimal(900) - Decimal(3) * k) * (r ** (k - 1))

def s_brute_force(r, n):
    return sum(f(r, k) for k in range(1, n + 1))

def s(r, n):
    a = Decimal(897)
    d = Decimal(-3)
    b = Decimal(1)

    if r == 1:
        first = a
        last = a - Decimal(3) * (n - 1)
        return Decimal(n) * (first + last) / 2
    
    p1 = (a * b - (a + n * d) * b * (r ** n)) / (1 - r)
    p2 = (d * b * r * (1 - (r ** n))) / (1 - r) ** 2
    return p1 + p2

n = 5000
target = Decimal(-6) * (Decimal(10) ** 11)
low, high = Decimal(0), Decimal(2)
EPS = Decimal("1e-13")

assert s(high, n) < target and s(low, n) > target
while high - low > EPS:
    mid = (low + high) / 2
    if s(mid, n) < target:
        high = mid
    else:
        low = mid
print(f"{(low + high) / 2:.12f}")
