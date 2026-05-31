from decimal import Decimal, getcontext
import decimal

getcontext().prec = 100

def f(n: int, S: int) -> int:
    sum_index = Decimal(n * (n + 1) // 2)
    p1 = (Decimal(S) / sum_index) ** sum_index
    p2 = Decimal(sum(k * Decimal(k).ln() for k in range(1, n + 1))).exp()
    return int((p1 * p2).to_integral_value(rounding=decimal.ROUND_FLOOR))

def P(n: int) -> int:
    return f(n, n)

print(sum(P(m) for m in range(2, 16)))