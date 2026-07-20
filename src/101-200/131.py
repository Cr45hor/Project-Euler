import gmpy2

N = 10**6

cnt = 0
k = 1
while True:
    n = (k + 1)**3 - k**3
    if n > N:
        break
    if gmpy2.is_prime(n):
        cnt += 1
    k += 1

print(cnt)
