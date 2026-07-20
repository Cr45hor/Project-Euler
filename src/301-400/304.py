import sympy as sp
import numpy as np

N = 10**5
SEED = sp.nextprime(10**14)
A = [SEED]
for n in range(1, N):
    A.append(sp.nextprime(A[-1]))

MOD = 1234567891011

M = np.array([
    [1, 1],
    [1, 0]
], dtype=object)
K = A[-1].bit_length() + 1
assert A[-1] < 2**K

M_pow2 = [M]
for k in range(1, K):
    next_M = M_pow2[-1] @ M_pow2[-1] % MOD
    M_pow2.append(next_M)

def F(n: int) -> int:
    if n <= 1:
        return n
    m = n - 1
    M_res = np.eye(2, dtype=object)
    while m > 0:
        msb = m.bit_length() - 1
        M_res = M_res @ M_pow2[msb] % MOD
        m ^= 1 << msb
    return M_res[0, 0]

print(sum(F(p) for p in A) % MOD)