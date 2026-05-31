from gmpy2 import is_prime
from itertools import combinations

def M(n: int, k: int, p: int) -> int:
    fact = [1] * p
    inv_fact = [1] * p

    # Precompute factorials and inverse factorials modulo p
    for i in range(2, p):
        fact[i] = fact[i - 1] * i % p
        
    # pow(x, -1, p) is equivalent to pow(x, p-2, p) in Python 3.8+ but mathematically clearer
    inv_fact[p - 1] = pow(fact[p - 1], -1, p)
    for i in range(p - 2, 0, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % p

    def C(n_i: int, k_i: int) -> int:
        assert 0 <= n_i < p and 0 <= k_i < p
        if k_i > n_i:
            return 0
        return (fact[n_i] * inv_fact[k_i] * inv_fact[n_i - k_i]) % p
    
    assert p**8 > n

    n_base = [0] * 8
    k_base = [0] * 8

    n_tmp = n
    k_tmp = k
    for i in range(8):
        n_base[i] = n_tmp % p
        k_base[i] = k_tmp % p
        n_tmp //= p
        k_tmp //= p
    
    result = 1
    for i in range(8):
        result = (result * C(n_base[i], k_base[i])) % p
    return result

if __name__ == "__main__":
    N = 10**18
    K = 10**9

    primes = [p for p in range(1000, 5000) if is_prime(p)]
    L = len(primes)
    binom_mod = [M(N, K, p) for p in primes]
    
    # --- 1. PRECOMPUTATION ---
    inv = {}
    a_inv = {}
    
    for i in range(L):
        a_i = binom_mod[i]
        p_i = primes[i]
        
        # Only do the heavy lifting if a_i is non-zero
        if a_i > 0:
            # inv_i[j] = (p_j)^-1 mod p_i
            inv_i = [pow(primes[j], -1, p_i) if i != j else 0 for j in range(L)]
            inv[i] = inv_i
            # Further optimize by pre-multiplying a_i into the array
            # a_inv[i][j] = a_i * (p_j)^-1 mod p_i
            a_inv[i] = [(a_i * val) % p_i for val in inv_i]

    total_sum = 0
    
    # --- 2. FAST INNER LOOP ---
    # itertools.combinations drops us into C-level speed for iterating
    for i, j, k in combinations(range(L), 3):
        a1, a2, a3 = binom_mod[i], binom_mod[j], binom_mod[k]
        
        # Original condition: Skip if all are zero
        if not a1 and not a2 and not a3:
            continue

        p1, p2, p3 = primes[i], primes[j], primes[k]
        res = 0
        
        # Add CRT terms ONLY if the numerator is non-zero
        # We also reduce modulo locally BEFORE multiplying by the large bases to keep numbers small
        if a1:
            res += ((a_inv[i][j] * inv[i][k]) % p1) * p2 * p3
        if a2:
            res += ((a_inv[j][i] * inv[j][k]) % p2) * p1 * p3
        if a3:
            res += ((a_inv[k][i] * inv[k][j]) % p3) * p1 * p2
            
        total_sum += res % (p1 * p2 * p3)

    print(total_sum)