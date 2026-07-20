import math

N = 50

EX = 1

EX2 = [0, 1]
EX3 = [0, 1]
for n in range(2, N + 1):
    next_EX2 = EX2[n - 1] + EX2[n - 2]
    next_EX3 = EX3[n - 1] + EX3[n - 2] * 3
    EX2.append(next_EX2)
    EX3.append(next_EX3)

mean = EX
var = EX2[N] - EX**2
std = math.sqrt(var)
skew = (EX3[N] - 3 * std**2 * mean - mean**3) / std**3
print(f"{skew:.8f}")

