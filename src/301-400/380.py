from decimal import Decimal, getcontext
import decimal

getcontext().prec = 100

def pi():
    """Compute Pi to the current precision.

    >>> print(pi())
    3.141592653589793238462643383

    """
    getcontext().prec += 2  # extra digits for intermediate steps
    three = Decimal(3)      # substitute "three=3.0" for regular floats
    lasts, t, s, n, na, d, da = 0, three, 3, 1, 0, 0, 24
    while s != lasts:
        lasts = s
        n, na = n+na, na+8
        d, da = d+da, da+32
        t = (t * n) / d
        s += t
    getcontext().prec -= 2
    return +s               # unary plus applies the new precision

PI = pi()

def cos(x):
    """Return the cosine of x as measured in radians.

    The Taylor series approximation works best for a small value of x.
    For larger values, first compute x = x % (2 * pi).

    >>> print(cos(Decimal('0.5')))
    0.8775825618903727161162815826
    >>> print(cos(0.5))
    0.87758256189
    >>> print(cos(0.5+0j))
    (0.87758256189+0j)

    """
    getcontext().prec += 2
    i, lasts, s, fact, num, sign = 0, 0, 1, 1, 1, 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i-1)
        num *= x * x
        sign *= -1
        s += num / fact * sign
    getcontext().prec -= 2
    return +s

cos_cache = {}

def precompute(N: int) -> None:
    for k in range(1, N):
        cos_cache[(k, N)] = cos(PI * k / N)

def st(n: int, m: int) -> Decimal:
    def coef(h: int, k: int) -> Decimal:
        return (Decimal(4) - Decimal(2) * cos_cache[(h, m)] - Decimal(2) * cos_cache[(k, n)]).log10()
    
    res_log10 = sum(coef(h, k) for h in range(1, m) for k in range(1, n))
    exponent = res_log10.to_integral_value(rounding=decimal.ROUND_FLOOR)
    mantissa = Decimal(10) ** (res_log10 - exponent)
    
    if exponent < 5:
        return (Decimal(10) ** res_log10).to_integral_value(rounding=decimal.ROUND_HALF_UP)
    else:
        return Decimal(f"{mantissa:.4f}e{exponent}")

if __name__ == "__main__":
    N = 100
    M = 500
    precompute(N)
    precompute(M)
    print(st(N, M))