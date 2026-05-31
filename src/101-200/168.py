def check(L: int, multiplier: int) -> list[int]:
    res = []
    for d in range(1, 10):
        p, r = divmod(d * (10**(L - 1) - multiplier), 10 * multiplier - 1)
        if r == 0 and len(str(p)) == L - 1:
            res.append(p * 10 + d)

    return res

MAX_LENGTH = 100
res = 0
for multiplier in range(1, 10):
    for L in range(2, MAX_LENGTH + 1):
        res += sum(check(L, multiplier))
print(res % (10**5))