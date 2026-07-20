import math
from typing import List

MOD = 10**9 + 7

def R1(q: int):
    if q % 4 != 1:
        return None
    p = 2**q - 1
    r = pow(q, pow(2, q - 2), p)
    return min(r, p - r)

def R2(q: int, take_min = True):
    if q % 4 != 1:
        return None
    p = 2**q - 1
    residues: set[int] = set()
    for x in range(1, q):
        a = x**2 % q
        residues.add(a)
    residues_list = list(residues)

    sgn: List[int] = [-1] * q
    for a in residues_list:
        sgn[a] = 1

    r = 0
    for a in range(1, q):
        r += sgn[a] * pow(2, a)

    assert r < p and r > -p, f"r = {r}, p = {p}"
    r %= p
    if r < 0:
        r += p
    if take_min:
        r = min(r, p - r)
    return r

def R3(q: int):
    if q % 4 != 1:
        return None
        
    # 1. Byte array for memory efficiency
    # Allocates exactly q bytes (74.2 MB for q = 74,207,281)
    is_res = bytearray(q)
    
    # 2. Compute quadratic residues using pure addition
    # Based on the property: (x+1)^2 = x^2 + 2x + 1
    sq = 0
    step = 1
    for _ in range((q + 1) // 2):
        is_res[sq] = 1
        sq += step
        if sq >= q:
            sq -= q
        step += 2
        
    # 3. Compute the sum using running powers of 2 
    r = 0
    p2 = 2
    for a in range(1, q):
        if is_res[a]:
            r += p2
        else:
            r -= p2
            
        # Efficiently compute next power of 2 without the modulo operator
        p2 += p2
        if p2 >= MOD:
            p2 -= MOD
            
    # # Python's modulo correctly handles negative values of r
    # return r % MOD

    guess_1 = r % MOD
    guess_2 = (pow(2, q, MOD) - 1 - guess_1) % MOD
    return guess_1, guess_2

# Q = [2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521, 607, 1279, 2203]
# for q in Q:
#     print(f"q = {q}")
#     print(f"    R1(q) = {R1(q)}")
#     print(f"    R2(q) = {R2(q)}")
#     assert R1(q) == R2(q), f"Mismatch for q = {q}: R1(q) = {R1(q)}, R2(q) = {R2(q)}"
#     if q % 4 == 1:
#         print(f"    R2(q, False) % MOD = {R2(q, False) % MOD}")
#         print(f"    R2(q, True) % MOD = {R2(q, True) % MOD}")
#         print(f"    R3(q) = {R3(q)}")

q = 74207281
print(R3(q))