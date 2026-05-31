import math

MOD = 10**9 + 7
N = 2022

fact = [1] * (N + 1)
inv_fact = [1] * (N + 1)

def precompute():
    for i in range(2, N + 1):
        fact[i] = fact[i - 1] * i % MOD
    
    inv_fact[N] = pow(fact[N], MOD - 2, MOD)
    for i in range(N - 1, 0, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD

def C(n: int, k: int) -> int:
    if k > n or k < 0:
        return 0
    return fact[n] * inv_fact[k] * inv_fact[n - k] % MOD

def E(N: int) -> int:
    assert N > 0
    if N == 1:
        return 9
    
    cap = 1 + (N // 2)
    res = 0

    for k in range(cap, N + 1):
        res += pow(9, N - k, MOD) * C(N, k) * 9 % MOD
        res %= MOD

    return res

def D(N: int) -> int:
    return sum(E(k) for k in range(1, N + 1)) % MOD

if __name__ == "__main__":
    precompute()
    print(D(N))
