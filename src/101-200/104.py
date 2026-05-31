from decimal import Decimal, getcontext
import decimal

getcontext().prec = 100

PHI = (Decimal(5).sqrt() + 1) / 2
MOD = 10**9

def G(n: int) -> Decimal:
    return PHI.log10() * n - Decimal(5).sqrt().log10()

def L(n: int) -> Decimal:
    return G(n).to_integral_exact(rounding=decimal.ROUND_CEILING)

def prefix(n: int, prefix_len: int) -> int:
    len = L(n)
    offset = len - prefix_len
    return int((Decimal(10) ** (G(n) - offset)).to_integral_exact(rounding=decimal.ROUND_FLOOR))

def is_pandigital(n: int) -> bool:
    s = str(n)
    return len(s) == 9 and set(s) == set("123456789")

if __name__ == "__main__":
    a = 442003280
    b = 839725641
    k = 541
    while True:
        k += 1
        c = (a + b) % MOD
        if is_pandigital(c) and is_pandigital(prefix(k, 9)):
            print(k)
            break
        a, b = b, c
