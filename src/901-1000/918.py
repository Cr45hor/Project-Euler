def a(n: int) -> int:
    if n <= 2:
        return n
    if n % 2 == 0:
        return a(n // 2) * 2
    else:
        m = (n - 1) // 2
        return a(m) - a(m + 1) * 3

def S(n: int) -> int:
    if n <= 1:
        return n
    if n % 2 == 0:
        return 4 - a(n // 2)
    else:
        return 4 - 3 * a((n + 1) // 2)
    
N = 10**12
print(S(N))