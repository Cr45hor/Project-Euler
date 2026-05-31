MOD = 10**9 + 7

def geometric_sum(r: int, n: int) -> int:
    if r == 1:
        return (n + 1) % MOD
    return (pow(r, n + 1, MOD) - 1) * pow(r - 1, MOD - 2, MOD) % MOD

N = 10**6
sum = MOD - N
for k in range(1, N + 1):
    sum = (sum + geometric_sum(1 - k**2, N)) % MOD
print(sum)