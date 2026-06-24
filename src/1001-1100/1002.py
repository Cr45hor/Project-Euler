with open("1002_input.txt", "r") as fin:
    arr = fin.readline().split(',')
    print(len(arr))

N = len(arr) // 2
match: dict[tuple[int, int]] = {}
pair_ind: dict[tuple[int, int]] = {}

for i, v in enumerate(arr):
    if v not in match:
        match[v] = i
    else:
        j = match[v]
        pair_ind[i] = j
        pair_ind[j] = i