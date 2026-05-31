from sympy import factorial

def belfry(n: int):
    perm = [x for x in range(n)]
    memo: set = set()
    memo.add(tuple(perm))
    perm_seq = [perm[:]]

    while True:
        inv_perm = [0] * n
        for pos, val in enumerate(perm):
            inv_perm[val] = pos
        found: bool = False

        # Start from the position of the largest element and move downwards
        for val in reversed(range(n)):
            pos = inv_perm[val]
            nxt_perm = []

            if pos > 0:
                # Swap with the left neighbor
                perm_left = perm[:]
                perm_left[pos], perm_left[pos - 1] = perm_left[pos - 1], perm_left[pos]
                if tuple(perm_left) not in memo:
                    nxt_perm.append(perm_left)
            
            if pos < n - 1:
                # Swap with the right neighbor
                perm_right = perm[:]
                perm_right[pos], perm_right[pos + 1] = perm_right[pos + 1], perm_right[pos]
                if tuple(perm_right) not in memo:
                    nxt_perm.append(perm_right)
            
            if nxt_perm:
                assert len(nxt_perm) == 1, "There should be exactly one valid next permutation"
                perm = nxt_perm[0]
                memo.add(tuple(perm))
                perm_seq.append(perm[:])
                found = True
                break
        
        if not found:
            break
    
    return perm_seq

N = 5
seq = belfry(N)
for i, perm in enumerate(seq):
    print(f"{i}: {perm}")
