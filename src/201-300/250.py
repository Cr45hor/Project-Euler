M = 250
L = 250250
MOD = 10**16

freq = [0] * M

def create_freq():
    for n in range(1, L + 1):
        freq[pow(n, n, M)] += 1

def poly_mul(A, B):
    C = [0] * M
    for i in range(M):
        if A[i] == 0:
            continue
        for j in range(M):
            if B[j] == 0:
                continue
            idx = (i + j) % M
            C[idx] = (C[idx] + A[i] * B[j]) % MOD
    return C

def poly_pow(poly, power):
    res = [0] * M
    res[0] = 1
    base = poly[:]
    
    while power > 0:
        if power % 2 == 1:
            res = poly_mul(res, base)
        base = poly_mul(base, base)
        power //= 2
        
    return res

def solve():
    create_freq()
    
    # Cache for (1+x)^n mod (x^250 - 1)
    # Dictionary mapping `n` -> resulting polynomial
    cache = {}
    
    # The base polynomial (1 + x)
    base_1_plus_x = [0] * M
    base_1_plus_x[0] = 1
    base_1_plus_x[1] = 1

    result_poly = [0] * M
    result_poly[0] = 1

    for r in range(M):
        n = freq[r]
        if n == 0:
            continue
            
        # 1. Fetch or calculate (1+x)^n
        if n not in cache:
            cache[n] = poly_pow(base_1_plus_x, n)
            
        source_poly = cache[n]
        
        # 2. Substitute x -> x^r
        term_poly = [0] * M
        for i in range(M):
            if source_poly[i] > 0:
                new_idx = (i * r) % M
                term_poly[new_idx] = (term_poly[new_idx] + source_poly[i]) % MOD
                
        # 3. Multiply into the running total
        result_poly = poly_mul(result_poly, term_poly)

    # Subtract the empty set
    ans = (result_poly[0] - 1) % MOD
    return ans

if __name__ == "__main__":
    print(solve())