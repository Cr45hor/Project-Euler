from itertools import permutations
from sympy import isprime

cnt = 0
for k in range(1, 10):
    for p in permutations('123456789', k):
        n = int(''.join(p))
        if isprime(n):
            # print(n)
            cnt += 1
print(f"Total: {cnt}")