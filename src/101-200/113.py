import math

N = 100

print(math.comb(N + 9, 9) + math.comb(N + 10, 10) - N * 10 - 2)