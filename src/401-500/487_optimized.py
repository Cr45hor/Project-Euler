from typing import List
import sympy as sp

def evaluate_lagrange(y: List[int], N: int, p: int) -> int:
    """
    Evaluates a polynomial of degree D at N modulo p, 
    given its values at x = 0, 1, ..., D in the array `y`.
    Time Complexity: O(D)
    """
    D = len(y) - 1
    
    # If N is within our precomputed points, just return it
    if N <= D:
        return y[N] % p
        
    # Precompute factorials and their modular inverses
    fact = [1] * (D + 1)
    for i in range(1, D + 1):
        fact[i] = (fact[i - 1] * i) % p
        
    inv_fact = [1] * (D + 1)
    inv_fact[D] = pow(fact[D], -1, p)
    for i in range(D - 1, -1, -1):
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % p
        
    # Precompute prefix and suffix products of (N - i)
    pref = [1] * (D + 1)
    suff = [1] * (D + 1)
    
    pref[0] = N % p
    for i in range(1, D + 1):
        pref[i] = (pref[i - 1] * (N - i)) % p
        
    suff[D] = (N - D) % p
    for i in range(D - 1, -1, -1):
        suff[i] = (suff[i + 1] * (N - i)) % p
        
    res = 0
    # Lagrange interpolation formula calculation
    for i in range(D + 1):
        # Numerator: product of (N - j) for all j != i
        num = 1
        if i > 0: 
            num = (num * pref[i - 1]) % p
        if i < D: 
            num = (num * suff[i + 1]) % p
            
        # Denominator: i! * (D - i)! * (-1)^(D - i)
        den = (inv_fact[i] * inv_fact[D - i]) % p
        if (D - i) % 2 == 1:
            den = (p - den) % p  # Apply negative sign modulo p
            
        term = (y[i] * num) % p
        term = (term * den) % p
        res = (res + term) % p
        
    return res

def F_mod_opt(k: int, N: int, p: int) -> int:
    """
    Computes sum_{i=1}^N i^k (mod p) in O(k log k)
    """
    D = k + 1 # Degree of F_k(N) is k + 1
    y = [0] * (D + 1)
    
    # Calculate initial points for x = 0, 1, ..., k+1
    for i in range(1, D + 1):
        y[i] = (y[i - 1] + pow(i, k, p)) % p
        
    return evaluate_lagrange(y, N, p)

def G_mod_opt(k: int, N: int, p: int) -> int:
    """
    Computes sum_{n=1}^N F(k, n) (mod p) in O(k log k)
    """
    D = k + 2 # Degree of G_k(N) is k + 2
    y = [0] * (D + 1)
    
    current_S = 0
    # Calculate initial points for x = 0, 1, ..., k+2
    for i in range(1, D + 1):
        current_S = (current_S + pow(i, k, p)) % p
        y[i] = (y[i - 1] + current_S) % p
        
    return evaluate_lagrange(y, N, p)

if __name__ == "__main__":
    k = 10000
    N = 10**12
    L = 2 * 10**9
    R = 2 * 10**9 + 2000
    p = sp.nextprime(L)
    res = 0

    while p <= R:
        res += G_mod_opt(k, N, p)
        p = sp.nextprime(p)
    print(res)