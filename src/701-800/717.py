def solve(limit: int) -> int:
    if limit < 3:
        return 0

    # 1. Fast bytearray Sieve of Eratosthenes (Odds only)
    half_n = limit // 2
    is_prime = bytearray([1]) * half_n
    is_prime[0] = 0  # Index 0 represents 1, which is not prime
    
    for i in range(1, int(limit**0.5) // 2 + 1):
        if is_prime[i]:
            # start maps to p^2, step maps to 2p
            start = 2 * i * (i + 1)
            step = 2 * i + 1
            # C-level slice assignment replacing the nested loop
            is_prime[start::step] = bytes(len(range(start, half_n, step)))

    total = 0

    # 2. Optimized calculation loop
    for i, v in enumerate(is_prime):
        if v:
            p = 2 * i + 1
            
            # e1 simplified with modulo
            e1 = (pow(2, p, p - 1) - 1) % (p - 1)
            
            # Extract common base calculation
            k = pow(2, e1, p)
            
            # p * p replaces p**2 for marginal parsing speedup
            s1 = k * pow(2, p, p * p) % (p * p)
            
            # Bitwise shift completely removes the s2 pow() call
            s2 = (k << 1) % p
            
            total += (s1 - s2) // p

    return total

def g(p: int) -> int:
    """Retained for assertion testing only."""
    e1 = (pow(2, p, p - 1) - 1) % (p - 1)
    k = pow(2, e1, p)
    s1 = k * pow(2, p, p * p) % (p * p)
    s2 = (k << 1) % p
    return (s1 - s2) // p

if __name__ == "__main__":
    assert g(3) == 2
    assert g(31) == 17
    
    N = 10**7
    print(solve(N))