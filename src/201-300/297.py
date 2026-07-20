from functools import lru_cache

TARGET = 10**17 - 1

F = [0, 1]
N = 2
while F[N - 2] + F[N - 1] <= TARGET:
    F.append(F[N - 2] + F[N - 1])
    N += 1

@lru_cache(maxsize=None)
def Z(n: int) -> int:
    if n == 0:
        return 0
    elif n == 1:
        return 1
    else:
        # Find the biggest F_k <= n
        k = 0
        while k + 1 < N and F[k + 1] <= n:
            k += 1
        return (n - F[k] + 1) + Z(F[k] - 1) + Z(n - F[k])
    
print(Z(TARGET))