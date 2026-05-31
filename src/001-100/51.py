from gmpy2 import is_prime
from itertools import combinations

THRESHOLD = 8

for n in range(10, 10**7):
    if is_prime(n):
        s = str(n)
        # Iterate over all combinations of indices to replace
        for r in range(1, len(s)):
            for indices in combinations(range(len(s)), r):
                family = []
                for digit in '0123456789':
                    # Skip leading zero case
                    if indices[0] == 0 and digit == '0':
                        continue
                    candidate = list(s)
                    for index in indices:
                        candidate[index] = digit
                    candidate_num = int(''.join(candidate))
                    if is_prime(candidate_num):
                        family.append(candidate_num)
                if len(family) >= THRESHOLD and n in family:
                    print(n)
                    exit()