import math

M = 73

sum = 0

for i in range(1, M + 1):
    for j in range(max(i + 1, M - i + 1), M + 1):
        assert 1 <= i < j <= M
        assert i + j > M
        if math.gcd(i, j) == 1:
            sum += 1.0 / (i * j)

print(sum)