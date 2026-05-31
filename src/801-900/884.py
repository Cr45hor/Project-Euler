import math

dp = {
    0: 0
}

def S(n: int) -> int:
    # print(f"S({n})")
    if n < 0:
        return 0
    
    if n in dp:
        return dp[n]
    
    # Find the biggest cube not exceeding n
    k = round(n ** (1/3))
    if k**3 > n:
        k -= 1
    assert k**3 <= n < (k+1)**3, f"Invalid k: {k} for n: {n}"

    k3 = k**3
    dp[n] = S(k3 - 1) + S(n - k3) + (n - k3 + 1)
    return dp[n]

# for n in range(1, 10):
#     print(S(n**3 - 1), end=' ')
for n in range(0, 40):
    print(S(n + 1) - S(n), end=' ')
print()

for n in range(2, 100):
    # if n == 8:
    #     print("Check", S(n) - S(n - 1), S(n - 1) - S(n - 2), (S(n) - S(n - 1)) != (S(n - 1) - S(n - 2) + 1))
    if (S(n) - S(n - 1)) != (S(n - 1) - S(n - 2) + 1):
        print(f"n: {n}, S(n) - S(n - 1): {S(n) - S(n - 1)}, S(n-1) - S(n - 2): {S(n-1) - S(n - 2)}")