A = 1777
B = 1855
M = 10**8

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

def tetration(a: int, b: int, mod: int) -> int:
    print(a, b, mod)
    if b == 1:
        return a % mod
    if mod == 1:
        return 0
    return pow(a, tetration(a, b - 1, phi(mod)), mod)

print(tetration(A, B, M))