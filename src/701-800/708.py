import math

N = 10**5
p = 2

cnt = 0
pk = 1
k = 0

while True:
    pk *= p
    k += 1
    if pk > N:
        break

    cnt += math.comb(k, 2) * ((N // pk) - (N // (pk * p)))

print(cnt)