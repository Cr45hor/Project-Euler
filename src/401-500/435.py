import numpy as np

MOD = 1307674368000  # (= 15!)
N = 10**15

def mat_power(mat: np.ndarray, n: int, modulo: int) -> np.ndarray:
    res = np.eye(len(mat), dtype=object)
    while n:
        if n & 1:
            res = (res @ mat) % modulo
        mat = (mat @ mat) % modulo
        n >>= 1
    return res

def F(n: int, x: int) -> int:
    mat = np.zeros((3, 3), dtype=object)
    mat[0][0] = mat[2][0] = x
    mat[0][1] = mat[2][1] = x * x
    mat[1][0] = mat[2][2] = 1

    res = mat_power(mat, n - 1, MOD)
    return (res[2][0] + res[2][2]) * x % MOD

if __name__ == "__main__":
    print(sum(F(N, x) for x in range(1, 101)) % MOD)