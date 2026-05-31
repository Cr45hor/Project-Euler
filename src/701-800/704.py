def v2(n: int) -> int:
    return n - n.bit_count()

def g(n: int, k: int) -> int:
    assert 0 <= k < n
    return v2(n) - v2(k) - v2(n - k)

def d(n: int) -> int:
    return n.bit_length() - (n & -n).bit_length()

def F(n: int) -> int:
    # return max(g(n, k) for k in range(n))
    return d(n + 1)

def G(n: int) -> int:
    assert n > 1
    msb = n.bit_length() - 1
    return (msb - 1) * (1 << msb) + 1 + (msb + 1) * (n - (1 << msb) + 1) - (n << 1) + n.bit_count()

def S(n: int) -> int:
    # return sum(F(k) for k in range(1, n + 1))
    return G(n + 1)

print(S(10**16))