from decimal import Decimal, getcontext

getcontext().prec = 100

N = 30
result = sum([(Decimal(2) ** (n - 1) - 1) / n for n in range(1, N + 1)])

# Round up this sum up to 2 decimal places
print(result.quantize(Decimal('0.01')))