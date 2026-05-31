import functools
from math import factorial

def build_permutation(n: int, step: int) -> list[int]:

    @functools.lru_cache(None)
    def dp(curr_n: int, curr_step: int) -> tuple[int, ...]:
        # Base Case
        if curr_n == 1:
            return (0,)
        
        candidates = []
        
        # The maximum possible step a prefix of length (curr_n - 1) can generate
        max_s = (1 << (curr_n - 2)) - 1 if curr_n > 1 else 0
        
        # Case 1: The last element is the max element (curr_n - 1) -> Contributes 0
        if curr_step <= max_s:
            pref = dp(curr_n - 1, curr_step)
            candidates.append(pref + (curr_n - 1,))
            
        # Case 2: The last element is k < curr_n - 1 -> Contributes 2^k
        for k in range(curr_n - 1):
            if curr_step >= (1 << k) and curr_step - (1 << k) <= max_s:
                # Build the valid prefix
                pref = dp(curr_n - 1, curr_step - (1 << k))
                
                # Shift all elements >= k up by 1 to make room for the new k
                new_pref = tuple(x + 1 if x >= k else x for x in pref)
                candidates.append(new_pref + (k,))
                
        return min(candidates)

    return list(dp(n, step))

def perm_index(perm: list[int]) -> int:
    idx = 0
    n = len(perm)
    perm_clone = perm[:]
    for i in range(n):
        idx += factorial(n - 1 - i) * perm_clone[i]
        for j in range(i, n):
            if perm_clone[j] > perm_clone[i]:
                perm_clone[j] -= 1
    return idx + 1 # Convert to 1-based index

def minimal_perm(step: int) -> int:
    ctz = (step & -step).bit_length() - 1
    step >>= ctz
    min_n = step.bit_length() + 1

    perm_suffix = build_permutation(min_n, step)
    return perm_index(perm_suffix)

print(minimal_perm(12 ** 12))
