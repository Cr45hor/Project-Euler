import math

def succ(n: int) -> int:
    return sum(math.factorial(int(digit)) for digit in str(n))
succ_list = [succ(n) for n in range(10 ** 7)]

MAX_CHAIN = 60
res = 0

for n in range(10 ** 6):
    visited = set()
    curr = n
    distinct_cnt = 0

    for i in range(MAX_CHAIN + 1):
        if curr in visited:
            distinct_cnt = i 
            break
        visited.add(curr)
        curr = succ_list[curr]

    if distinct_cnt == 60:
        res += 1

print(res)