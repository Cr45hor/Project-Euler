from decimal import Decimal, getcontext
import decimal
import bisect

getcontext().prec = 200

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

PI: Decimal = Decimal(pi())

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

def f(k: int, n: int) -> Decimal:
    return (Decimal(k) / Decimal(n)).exp() - 1 - PI/4

def g(n: int):
    arr = [f(k, n) for k in range(n * 2)]
    arr_paired = [(arr[i] + arr[j], i, j) for i in range(n * 2) for j in range(i + 1, n * 2)]
    arr_paired.sort(key=lambda x: x[0])
    pair_sums = [item[0] for item in arr_paired]

    def merge(p1: tuple[Decimal, int, int], p2: tuple[Decimal, int, int]) -> tuple[Decimal, int, int, int, int]:
        sum_ij, i, j = p1
        sum_kl, k, l = p2
        return sum_ij + sum_kl, i, j, k, l

    def match(sum_ij: Decimal, i: int, j: int) -> tuple[Decimal, int, int, int, int]:
        target = -sum_ij
        id_l = bisect.bisect_left(pair_sums, target)
        id_r = bisect.bisect_right(pair_sums, target)
        assert id_l == id_r
        id = id_l

        if id == 0:
            return merge((sum_ij, i, j), arr_paired[0])
        elif id == n * 2:
            return merge((sum_ij, i, j), arr_paired[-1])
        else:
            cand1 = merge((sum_ij, i, j), arr_paired[id - 1])
            cand2 = merge((sum_ij, i, j), arr_paired[id])
            return cand1 if abs(cand1[0]) < abs(cand2[0]) else cand2

    return tuple(sorted(min((match(sum_ij, i, j) for sum_ij, i, j in arr_paired), key=lambda x: abs(x[0]))[1:]))

if __name__ == "__main__":
    N = 10000
    print(g(N))