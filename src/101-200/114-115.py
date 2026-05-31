def calc(N: int, M: int) -> int:
    dp = [0] * (N + 1)
    s = [0] * (N + 1)
    ss = [0] * (N + 1)

    dp[M] = s[M] = ss[M] = 1

    for i in range(M + 1, N + 1):
        dp[i] = 1 + i - M + ss[i - M - 1]
        s[i] = s[i - 1] + dp[i]
        ss[i] = ss[i - 1] + s[i]

    return s[N] + 1

if __name__ == "__main__":
    print("Problem 114:", calc(50, 3))

    print("Problem 115:", end=" ")
    M = 50
    N = M
    while calc(N, M) < 1_000_000:
        N += 1
    print(N)