import math
from typing import List

A = 1504170715041707
MOD = 4503599627370517
inv = pow(A, -1, MOD)
assert math.gcd(A, MOD) == 1

# (pos, val) = (pos, A * pos % MOD)
coins: List[tuple[int, int]] = []

# Find a reasonable limit so that A * limit < sqrt(MOD) mod MOD
cap = math.isqrt(MOD)
limit = 1
while A * limit % MOD >= cap:
    val = A * limit % MOD
    coins.append((limit, val))
    limit += 1

# Directly find the positions of the coins that are less than the limit
small_cap = A * limit % MOD
for val in range(1, small_cap + 1):
    pos = val * inv % MOD
    coins.append((pos, val))

coins.sort()
sum = 0
min_coin = MOD
for pos, val in coins:
    if val < min_coin:
        sum += val
        min_coin = val
print(sum)