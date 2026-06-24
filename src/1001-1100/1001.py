arr = [0, 1, 2, 3, 1, 4, 0, 5, 4, 2, 6, 7, 3, 8, 6, 5, 9, 8, 9, 7]
MOD = 1003443221

with open("1001_input.txt", "r") as fin:
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

dp: dict[tuple[tuple[int, int], int]] = {}

def solve(l: int, r: int) -> int:
    if l > r:
        return 1
    
    if (l, r) in dp:
        return dp[(l, r)]
    
    res = 0

    # Case 1: Match l with pair_ind[l]
    # Only possible if l < pair_ind[l] <= r
    # Split [l, r] into [l + 1, pair_ind[l] - 1] and [pair_ind[l] + 1, r]
    if l < pair_ind[l] <= r:
        res += solve(l, pair_ind[l] - 1) * solve(pair_ind[l] + 1, r)
    
    # Case 2: Skip l
    res += solve(l + 1, r)

    dp[(l, r)] = res
    return res

print(solve(0, N * 2 - 1))
    
