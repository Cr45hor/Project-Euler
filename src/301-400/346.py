N = 10**12
L = (N + 1).bit_length() - 2

repunit: list[int] = []

for l in range(2, L + 1):
    n = 2
    while True:
        x = (n ** (l + 1) - 1) // (n - 1)
        if x >= N:
            break
        repunit.append(x)
        n += 1

strong_repunit = set(repunit)
strong_repunit.add(1)
# strong_repunit = sorted(strong_repunit)
print(strong_repunit)
print(len(strong_repunit))
print(sum(strong_repunit))