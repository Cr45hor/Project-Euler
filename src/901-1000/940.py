import numpy as np

MOD = 1123581313

N = 50
f: list[int] = [0] * (N + 1)

def precompute_fibo():
    f[0] = 0
    f[1] = 1
    for i in range(2, N + 1):
        f[i] = f[i - 1] + f[i - 2]

"""
A(0, 0) = 0
A(0, 1) = A(1, 0) = 1
A(0, n) = A(0, n - 1) + 3A(0, n - 2) (n > 1)
A(m, 0) = 3A(m - 1, 0) + A(m - 2, 0) (m > 1)
A(m, n) = A(m, n - 1) * 3 - A(m - 1, n) (m, n > 0)

Solved for general m, n:
A(m, n) = A(m, 0)A(0, n + 1) + A(m + 1, 0)A(0, n) - 2A(m, 0)A(0, n)
"""

def mat_mul(M: np.ndarray, k: int, mod: int = MOD) -> np.ndarray:
    # Assert M is a square matrix
    assert M.shape[0] == M.shape[1]
    n = M.shape[0]

    res = np.eye(n, dtype=object)
    base = M.copy()
    while k > 0:
        if k & 1:
            res = np.dot(res, base) % mod
        base = np.dot(base, base) % mod
        k >>= 1
    return res

def F(m: int) -> int:
    # A(m, 0) = 3A(m - 1, 0) + A(m - 2, 0) (m > 1)
    # Let A1(m) = A(m, 0)
    # Then A1(m) = 3A1(m - 1) + A1(m - 2) with A1(0) = 0, A1(1) = 1
    # Using matrix exponentiation to compute A1(m) 

    if m <= 1:
        return m
    
    T = np.array([
        [3, 1],
        [1, 0]
    ], dtype=object)

    T_res = mat_mul(T, m - 1)
    return T_res[0][0]

def G(n: int) -> int:
    # A(0, n) = A(0, n - 1) + 3A(0, n - 2) (n > 1)
    # Let A2(n) = A(0, n)
    # Then A2(n) = A2(n - 1) + 3A2(n - 2) with A2(0) = 0, A2(1) = 1
    # Using matrix exponentiation to compute A2(n)

    if n <= 1:
        return n
    
    T = np.array([
        [1, 3],
        [1, 0]
    ], dtype=object)

    T_res = mat_mul(T, n - 1)
    return T_res[0][0]

if __name__ == "__main__":
    precompute_fibo()

    U = sum(F(f[i]) for i in range(2, N + 1))
    V = sum(F(f[i] + 1) for i in range(2, N + 1))
    X = sum(G(f[i]) for i in range(2, N + 1))
    Y = sum(G(f[i] + 1) for i in range(2, N + 1))
    result = U * Y + V * X - U * X * 2

    print(result % MOD)
    