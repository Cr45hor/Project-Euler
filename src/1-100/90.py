from itertools import combinations

res = 0

squares = set([n * n for n in range(1, 10)])

for D1 in combinations(range(10), 6):
    D1_list = list(D1)
    D1_set = set(D1_list)
    if 6 in D1_set:
        D1_set.add(9)
    if 9 in D1_set:
        D1_set.add(6)
    
    for D2 in combinations(range(10), 6):
        D2_list = list(D2)
        D2_set = set(D2_list)
        if 6 in D2_set:
            D2_set.add(9)
        if 9 in D2_set:
            D2_set.add(6)

        two_digits = set()
        for d1 in D1_set:
            for d2 in D2_set:
                two_digits.add(d1 * 10 + d2)
                two_digits.add(d2 * 10 + d1)

        if squares.issubset(two_digits):
            res += 1

print(res // 2)

        