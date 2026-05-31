import math

THRESHOLD = 10 ** 6
res = 0

for n in range(23, 101):
    max_k = n >> 1
    for k in range(0, max_k + 1):
        c = math.comb(n, k)
        if c > THRESHOLD:
            res += (max_k - k + 1) * 2 - ((n + 1) & 1)
            break

print(res)