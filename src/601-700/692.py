from functools import lru_cache

TARGET = 23416728348467685

F = [0, 1]
n = 2
while F[n - 2] + F[n - 1] <= TARGET:
    F.append(F[n - 2] + F[n - 1])
    n += 1

assert F[n - 1] == TARGET

@lru_cache(maxsize=None)
def G(id: int) -> int:
    assert 2 <= id < n
    if id == 2:
        return 1
    elif id == 3:
        return 3
    else:
        return G(id - 1) + G(id - 2) + F[id - 1]
    
assert F[7] == 13 and G(7) == 43
print(G(n - 1))