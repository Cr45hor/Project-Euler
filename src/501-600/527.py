from typing import List
import numpy as np

def R(n: int) -> float:
    dp = np.zeros((n + 1, n + 1), dtype=np.float64)

    dp[1][1] = 1.0
    for l in range(2, n + 1):
        for t in range(1, l + 1):
            for g in range(1, l + 1):
                if g == t:
                    continue
                elif g < t:
                    dp[l][t] += dp[l - g][t - g]
                else:
                    assert g > t
                    dp[l][t] += dp[g - 1][t]
            dp[l][t] = 1 + dp[l][t] / l
    return np.mean(dp[n][1:])

# R(0) = 0
# R(n) = 1 + 1/n * (\sum_{g = 1}^{n} T(g - 1)*(g - 1)/n + T(n - g)*(n - g)/n)
def R_reduced(n: int) -> float:
    # H_n = \sum_{k=1}^{n} 1/k
    if n == 0:
        return 0
    H_n = sum(1 / k for k in range(1, n + 1))
    return 2 * (1 + 1/n) * H_n - 3

def B_reduced(n: int) -> float:
    k = n.bit_length() - 1
    assert 2**k <= n < 2**(k + 1)
    return 1 + ((n + 1) * k + 2 - 2**(k + 1)) / n

N = 10**10
print(f"{R_reduced(N) - B_reduced(N):.8f}")