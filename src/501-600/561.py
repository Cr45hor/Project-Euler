def F(m: int, n: int) -> int:
    return ((n + 1) * (n + 2) // 2) ** m - (n + 1) ** m

def highest_pow2(x: int) -> int:
    return (x & -x).bit_length() - 1

def E(m: int, n: int) -> int:
    res = highest_pow2(n + 1) * m - m
    if n & 1:
        pass
    else:
        res += m
        if n % 4 == 0:
            n_divided = (n >> 1) + 1
            if m & 1:
                res += highest_pow2(n_divided - 1) + highest_pow2(m)
            else:
                res += highest_pow2(n_divided**2 - 1) + highest_pow2(m) - 1
    return res

M = 904961

def sum_v2(n: int) -> int:
    # v2(n!) = v2(2) + ... + v2(n)
    return n - n.bit_count()

def Q(n: int) -> int:
    res = sum_v2(n + 1) * M - M * n
    res += (n // 2) * M

    cnt_multiply_4 = n // 4
    assert M & 1
    res += highest_pow2(M) * cnt_multiply_4 + cnt_multiply_4 + sum_v2(cnt_multiply_4)

    return res

assert Q(8) == 2714886
print(Q(10**12))


