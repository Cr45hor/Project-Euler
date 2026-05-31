from decimal import Decimal, getcontext
import math

getcontext().prec = 100

def P(k, n):
    return (Decimal(1) - (Decimal(1) / Decimal(2)) ** k) ** n

# result = sum(Decimal(k) * (P(k, 32) - P(k - 1, 32)) for k in range(1, 100000))
result = sum(math.comb(32, k) * Decimal(-1)**(k + 1) / (Decimal(1) - (Decimal(1) / Decimal(2))**k) for k in range(1, 33))
result = result.quantize(Decimal('1.0000000000'))
print(result)