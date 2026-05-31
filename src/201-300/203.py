from typing import Literal

N = 51

spf = [0] * N

def sieve():
    global spf
    for i in range(N):
        spf[i] = i

    for i in range(2, N):
        if i * i > N:
            break
        if spf[i] == i:
            for j in range(i * i, N, i):
                spf[j] = min(spf[j], i)

def factorize(x: int) -> dict[int, int]:
    assert x >= 1 and x < N
    if x == 1:
        return {}

    factors = {}
    while x > 1:
        p = spf[x]
        k = 0
        while spf[x] == p:
            x //= p
            k += 1
        factors[p] = k
    return factors

def update_product(curr: dict[int, int], x: int, sign: Literal[1, -1]):
    x_factors = factorize(x)
    for p, k in x_factors.items():
        curr[p] = curr.get(p, 0) + sign * k
        if curr[p] == 0:
            del curr[p]

def check_squarefree(prod: dict[int, int]) -> bool:
    for k in prod.values():
        if k > 1:
            return False
    return True

def convert_prod(prod: dict[int, int]) -> int:
    ans = 1
    for p, k in prod.items():
        ans *= p ** k
    return ans

if __name__ == '__main__':
    sieve()

    squarefree_comb = set()
    for n in range(2, N):
        prod = {}
        for k in range(1, n + 1):
            update_product(prod, n - k + 1, 1)
            update_product(prod, k, -1)
            if check_squarefree(prod):
                squarefree_comb.add(convert_prod(prod))

    print(sum(squarefree_comb))
    