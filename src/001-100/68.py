from itertools import permutations

solutions = []

N = 5

for p in permutations(range(1, N * 2 + 1)):
    segs = []
    for i in reversed(range(N)):
        a = p[i + N]
        b = p[(i + 1) % N]
        c = p[i]
        segs.append((a, b, c))

    min_index = min(range(N), key=lambda i: segs[i][0])
    segs = segs[min_index:] + segs[:min_index]

    if all(sum(seg) == sum(segs[0]) for seg in segs):
        result = ''.join(str(num) for seg in segs for num in seg)
        if len(result) <= 16:
            solutions.append(result)

print(max(solutions))