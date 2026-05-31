def phi(x: int) -> int:
    if x == 1:
        return 1
    if x < 4:
        return x - 1
    
    result = x
    if x % 2 == 0:
        result -= result // 2
        while x % 2 == 0:
            x >>= 1
    
    p = 3
    while p * p <= x:
        if x % p == 0:
            result -= result // p
            while x % p == 0:
                x //= p
        p += 2
    
    if x > 1:
        result -= result // x
    return result

def check_greater(a: int, b: int, M: int) -> bool:
    assert M < 2*64
    if b > 64:
        return True
    return a**b >= M

def hyperoperation(n: int, a: int, b: int, MOD: int, THRESHOLD: int) -> int:
    if n == 1:
        return pow(a, b, MOD), check_greater(a, b, THRESHOLD)
    # TODO: Complete this function
    print(f"hyperoperation({n}, {a}, {b}, {MOD}, {THRESHOLD})")

    E = hyperoperation(n, a, b - 1, MOD, THRESHOLD)
    raise NotImplementedError

def A(m: int, n: int, MOD: int) -> int:
    if m == 0:
        return (n + 1) % MOD
    if m == 1:
        return (n + 2) % MOD
    if m == 2:
        return (2 * n + 3) % MOD
    
    assert m > 2
    return (hyperoperation(m - 2, 2, n + 3, MOD, MOD) + MOD - 3) % MOD

M = 14**8
print(A(4, 4, M))