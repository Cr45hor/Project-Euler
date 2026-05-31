def f(n: int, k: int) -> int:
    d = n - k
    res = n
    for b in range(d.bit_length()):
        if (d >> b) & 1:
            n -= 1 << b
            res += n
    assert n == k
    return res

print(f(10**17, 9**17))