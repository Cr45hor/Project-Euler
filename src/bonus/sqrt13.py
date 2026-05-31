from decimal import Decimal, getcontext
import decimal

getcontext().prec = 2000

D = 1000
N = 13

S = Decimal(N).sqrt()
S_frac = S - S.to_integral_value(context=decimal.Context(rounding=decimal.ROUND_FLOOR))
res = sum(int(d) for d in str(S_frac)[2:2+D])
print(res)