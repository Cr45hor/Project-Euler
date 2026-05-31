from decimal import Decimal, getcontext
import decimal

getcontext().prec = 100

def gen(N: int, theta: Decimal) -> list[Decimal]:
    b: list[Decimal] = [Decimal(0)] * N
    a: list[Decimal] = [Decimal(0)] * N

    b[0] = theta
    for i in range(1, N):
        dec_part = b[i - 1].to_integral_exact(rounding=decimal.ROUND_FLOOR)
        frac_part = b[i - 1] - dec_part
        b[i] = dec_part * (frac_part + 1)
    
    for i in range(N):
        a[i] = b[i].to_integral_exact(rounding=decimal.ROUND_FLOOR)
    
    return a

def concat(a: list[Decimal]) -> str:
    result: str = str(a[0].to_integral_exact(rounding=decimal.ROUND_FLOOR)) + '.'
    for x in a[1:]:
        result += str(x.to_integral_exact(rounding=decimal.ROUND_FLOOR))
    return result

N = 20
dx = Decimal(10) ** (-50)

L = Decimal(2) + dx
R = Decimal(3) - dx

while R - L > dx:
    mid = (L + R) / 2
    a = gen(N, theta=mid)
    tau = concat(a)
    if Decimal(tau) > mid:
        L = mid
    else:
        R = mid

# Print L up to 24 digits after the decimal point
print(f"{L:.24f}")
