import math
import numpy as np
from typing import List
from functools import reduce
import operator

MOD = 10**9 + 7
rng = np.random.default_rng()

def sieve(n: int) -> List[int]:
    spf = [1] * (n + 1)
    for i in range(2, n + 1, 2):
        spf[i] = 2
    for i in range(3, n + 1, 2):
        spf[i] = i
    for i in range(3, math.isqrt(n) + 1, 2):
        if spf[i] == i:
            for j in range(i * i, n + 1, i * 2):
                if spf[j] == j:
                    spf[j] = i
    return spf

def factorize(n: int, spf: List[int]) -> List[int]:
    factors = []
    while n > 1:
        factors.append(spf[n])
        n //= spf[n]
    return factors

def C(L: int, R: int) -> int:
    assert 0 <= L <= R

    spf = sieve(R)
    print("Number of primes:", sum(1 for x in range(L, R + 1) if spf[x] == x))  # Count primes in the range
    prime_mask = rng.integers(0, 2**64, R + 1, dtype=np.uint64)

    int_mask: List[int] = []
    set_prime_factors = set()
    for x in range(L, R + 1):
        factors = factorize(x, spf)
        set_prime_factors.update(factors)
        mask = reduce(operator.xor, (prime_mask[p] for p in factors), 0)
        int_mask.append(mask)

    print(len(set_prime_factors), "distinct prime factors in the range")  # Count distinct prime factors

    basis = []
    for mask in int_mask:
        for axis in basis:
            mask = min(mask, mask ^ axis)
        if mask > 0:
            basis.append(mask)
    
    dim_ker = (R - L + 1) - len(basis)
    print(f"Dimension of kernel: {dim_ker}") 
    # print(f"Basis: {basis}")
    print(f"Intended solution: {pow(2, dim_ker, MOD) - 1}")
    return pow(2, dim_ker, MOD) - 1


if __name__ == "__main__":
    assert C(40, 55) == 15
    # assert C(1000, 1234) == 975523611
    print(C(1000, 1234))
    print(C(1000000,1234567))