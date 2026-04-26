import numpy as np

MOD = 10 ** 9

A0 = np.array([[1, 0],
               [-1, 2]], dtype=np.int64)
A1 = np.array([[0, 1],
               [-2, 3]], dtype=np.int64)

I = np.identity(2, dtype=np.int64)
O = np.zeros((2, 2), dtype=np.int64)

T0 = np.block([[A0 + A1, -A1, I],
               [O, A0, O],
               [O, O, I]])
T1 = np.block([[A0 + A1, O, I],
               [O, A1, O],
               [O, O, I]])
T = [T0, T1]

W1 = np.array([1, 3, 1, 3, 1, 3], dtype=np.int64)

def S(n: int) -> np.ndarray:
    W_res = W1
    for bit in range(n.bit_length() - 2, -1, -1):
        r = (n >> bit) & 1
        W_res = T[r] @ W_res % MOD
    return W_res

print(S(3 ** 37)[0])
