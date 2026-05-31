from itertools import count, permutations

def check(num: int) -> int:
    digit_len = 0
    min_n = 1
    for n in range(1, 10):
        digit_len += len(str(num * n))
        if digit_len >= 9:
            min_n = n
            break
    
    L = [str(num * n) for n in range(1, min_n + 1)]
    concat_prod = ''.join(L)
    return (int(concat_prod) if digit_len == 9 and set(concat_prod) == set('123456789') else -1)

results: list[int] = [check(num) for num in range(1, 10**5)]
print(max(results))