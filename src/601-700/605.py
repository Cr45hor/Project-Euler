import math

def f(n: int):
    assert n >= 1
    return (n - 1) / (2**n)

def P(n: int, k: int):
    assert n >= 1
    assert 1 <= k <= n
    d = n - k
    scale = 2**d
    num = (k - 1) * (2**n - 1) + n
    den = (2**n - 1)**2
    return scale * num / den
    
MOD = 10**8

def M_mod(n: int, k: int):
    assert 1 <= k <= n
    g = math.gcd(n, pow(2, n, n) - 1)
    N_mod = pow(2, n, MOD) 
    scale = pow(2, n - k, MOD)
    base_num = ((k - 1) * (N_mod - 1) + n) % MOD
    den = ((N_mod - 1)**2) % MOD
    return (scale * base_num * den * pow(g, -1, MOD)**2) % MOD

assert M_mod(3, 1) == 588 and M_mod(6, 2) == 486864

N = 10**8 + 7
K = 10**4 + 7
result = M_mod(N, K)
print(result)