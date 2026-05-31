import math

MOD = 10**9 + 7

def S(a: int, n: int) -> int:
    return sum((-1)**(k + 1) * math.comb(a, k) * (a - k)**n for k in range(1, a + 1))

def geometric_sum(a: int, n: int) -> int:
    # Sum of 1 + a + a^2 + ... + a^n
    if a == 1:
        return n + 1
    return (pow(a, n + 1, MOD) - 1) * pow(a - 1, MOD - 2, MOD) % MOD

def I(a: int, n: int) -> int:
    fact = [1] * (a + 1)
    inv_fact = [1] * (a + 1)
    for i in range(2, a + 1):
        fact[i] = fact[i - 1] * i % MOD
    inv_fact[a] = pow(fact[a], MOD - 2, MOD)
    for i in range(a - 1, 0, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD
    inv_fact[0] = 1

    return sum((-1)**(k + 1) * math.comb(a, k) * geometric_sum(a - k, n) for k in range(1, a + 1)) % MOD

if __name__ == "__main__":
    a = 10**7
    n = 10**12
    print(I(a, n))

