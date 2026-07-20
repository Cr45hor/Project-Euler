import math

THRESHOLD = 1/12345

def x_max(M: int) -> int:
    exact_x_max = (1 + math.sqrt(1 + M * 4)) / 2
    return math.floor(exact_x_max)

def t_max(M: int) -> int:
    return math.floor(math.log2(x_max(M)))

def P(M: int) -> float:
    return t_max(M) / (x_max(M) - 1)

L = 440 * 10**8
R = 441 * 10**8
assert P(L) >= THRESHOLD 
assert P(R) < THRESHOLD

for m in range(L, R + 1):
    if P(m) < THRESHOLD:
        print(m)
        break