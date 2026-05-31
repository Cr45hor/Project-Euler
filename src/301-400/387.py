from gmpy2 import is_prime

THRESHOLD = 10**14

harshad = []
strong_harshad = []

def dfs(n: int, s: int) -> None:
    if n >= THRESHOLD:
        return
    else:
        harshad.append(n)
        if is_prime(n // s):
            strong_harshad.append(n)
    
    for d in range(10):
        next_n = n * 10 + d
        next_s = s + d
        if next_n % next_s == 0:
            dfs(next_n, next_s)

for d in range(1, 10):
    dfs(d, d)

sum = 0
for n in strong_harshad:
    for d in range(1, 10, 2):
        candidate = n * 10 + d
        if is_prime(candidate) and candidate < THRESHOLD:
            sum += candidate
print(sum)