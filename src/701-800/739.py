import math

MOD = 10**9 + 7
N = 10**8

def coef(n: int, k: int) -> int:
    return math.comb(n + k, k) - math.comb(n + k, k - 1) if k > 0 else 1

fact = [1] * (N * 2)
inv_fact = [1] * (N * 2)

def precompute():
    for i in range(2, N * 2):
        fact[i] = (fact[i - 1] * i) % MOD
    inv_fact[N * 2 - 1] = pow(fact[N * 2 - 1], MOD - 2, MOD)
    for i in range(N * 2 - 2, 0, -1):
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % MOD

def C(n: int, k: int) -> int:
    if k < 0 or k > n:
        return 0
    return (fact[n] * inv_fact[k] % MOD) * inv_fact[n - k] % MOD

def w(n: int, k: int) -> int:
    return C(n * 2 - k - 2, n - k) - C(n * 2 - k - 2, n - k - 1) if k > 0 else 1

def main():
    precompute()

    a = 1
    b = 3
    res = w(N, 1) * a + w(N, 2) * b
    for n in range(3, N + 1):
        c = a + b
        res = (res + w(N, n) * c) % MOD
        a, b = b, c
    print(res)

if __name__ == "__main__":
    main()