from itertools import permutations

valid_products = set()

for p in permutations('123456789'):
    for len_a in range(1, 5):
        for len_b in range(1, 6 - len_a):
            a = int(''.join(p[:len_a]))
            b = int(''.join(p[len_a:len_a + len_b]))
            c = int(''.join(p[len_a + len_b:]))
            if a * b == c:
                valid_products.add(c)

print(sum(valid_products))