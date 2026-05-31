import os
import math
import itertools
import time

CACHE_FILE = "min_s_cache.txt"
MAX_PRIME_SIEVE = 2000000

# 1. Fast Prime Sieve for instant lookups
is_p = [True] * MAX_PRIME_SIEVE
is_p[0] = is_p[1] = False
for i in range(2, int(MAX_PRIME_SIEVE**0.5) + 1):
    if is_p[i]:
        for j in range(i*i, MAX_PRIME_SIEVE, i):
            is_p[j] = False
primes = [i for i, val in enumerate(is_p) if val]

def is_prime_dynamic(n):
    """Fallback primality test if we exceed the 2M sieve."""
    if n < MAX_PRIME_SIEVE: return is_p[n]
    if n % 2 == 0 or n % 3 == 0: return False
    for i in range(5, int(n**0.5) + 1, 6):
        if n % i == 0 or n % (i + 2) == 0: return False
    return True

def get_factorizations(n, start_factor=2):
    """Unordered multiplicative factorizations."""
    res = [[n]]
    for i in range(start_factor, int(n**0.5) + 1):
        if n % i == 0:
            for sub in get_factorizations(n // i, i):
                res.append([i] + sub)
    return res

def primitive_root(p):
    """Finds the smallest primitive root generator for p."""
    if p == 2: return 1
    phi = p - 1
    factors = []
    n = phi
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            factors.append(i)
            while n % i == 0: n //= i
    if n > 1: factors.append(n)

    for res in range(2, p):
        valid = True
        for f in factors:
            if pow(res, phi // f, p) == 1:
                valid = False; break
        if valid: return res
    return None

def load_cache():
    cache = {}
    if os.path.exists(CACHE_FILE):
        with open(CACHE_FILE, "r") as f:
            for line in f:
                if ":" in line:
                    p_str, s_str = line.strip().split(":", 1)
                    cache[int(p_str.strip())] = int(s_str.strip())
    return cache

def save_to_cache(p, s):
    with open(CACHE_FILE, "a") as f:
        f.write(f"{p}: {s}\n")

def find_minimum_s(p):
    if p == 2: return 1
    
    g = primitive_root(p)
    factorizations = get_factorizations(p - 1)
    
    # Pre-bucket the smallest prime for every possible residue modulo p
    # This turns prime searching into an O(1) array lookup
    first_prime_for_mod = [None] * p
    missing = p - 1
    for q in primes:
        rem = q % p
        if rem != 0 and first_prime_for_mod[rem] is None:
            first_prime_for_mod[rem] = q
            missing -= 1
            if missing == 0: break

    min_s = float('inf')

    for fact in factorizations:
        # Test every unique permutation of the subset sizes
        for perm in set(itertools.permutations(fact)):
            s_val = 1
            W = 1
            
            for c in perm:
                best_q = float('inf')
                
                # Algebraic conditions: L = W * k, where gcd(k, c) == 1
                for k in range(1, (p - 1) // W + 1):
                    if math.gcd(k, c) == 1:
                        # Calculate the required residue modulo p
                        req_res = pow(g, W * k, p)
                        
                        # O(1) lookup for the absolute smallest prime with this residue
                        q = first_prime_for_mod[req_res]
                        
                        # Dynamic fallback if prime > 2,000,000 (extremely rare)
                        if q is None:
                            q = req_res if req_res >= 2 else req_res + p
                            while not is_prime_dynamic(q): q += p
                            first_prime_for_mod[req_res] = q
                            
                        if q < best_q:
                            best_q = q
                            
                s_val *= best_q ** (c - 1)
                W *= c
                
            if s_val < min_s:
                min_s = s_val

    return min_s

def main(N):
    print(f"Loading cache and calculating minimum 's' for primes < {N}...\n")
    cache = load_cache()
    
    start_time = time.time()
    
    for p in primes:
        if p >= N: break
        
        if p in cache:
            s = cache[p]
            print(f"{p:<10} | {s} (Cached)")
        else:
            s = find_minimum_s(p)
            save_to_cache(p, s)
            cache[p] = s
            # Format output to gracefully handle massive BigInts
            s_str = str(s)
            if len(s_str) > 30:
                s_str = f"{s_str[:5]}...{s_str[-5:]} ({len(s_str)} digits)"
            print(f"{p:<10} | {s_str}")

    print(f"\nExecution time: {time.time() - start_time:.4f} seconds")

if __name__ == "__main__":
    main(20000)