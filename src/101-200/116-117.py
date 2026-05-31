def calc(n: int, tiles: list[int]) -> int:
    dp = [0] * (n + 1)
    dp[0] = 1
    for i in range(1, n + 1):
        dp[i] = dp[i - 1]
        for t in tiles:
            if i - t >= 0:
                dp[i] += dp[i - t]
    return dp[n]

N = 50
tiles = [2, 3, 4]
print("Problem 116:", sum(calc(N, [t]) - 1 for t in tiles))
print("Problem 117:", calc(N, tiles))