import time

# Target Prime and max tree depth
p = 137
MAX_DEPTH = 8

# Global state to track maximums
max_J = 0
max_M = 0

# Precompute powers of p for tracking p-adic precision
MOD = [1] * (MAX_DEPTH + 1)
for i in range(1, MAX_DEPTH + 1):
    MOD[i] = MOD[i - 1] * p

def mod_inverse(a, m):
    return pow(a, -1, m)

def sum_inverse_powers(m, mod_val):
    """Compute sum_{i=1}^{p-1} (1 / i^m) mod mod_val"""
    total = 0
    for i in range(1, p):
        inv = mod_inverse(i, mod_val)
        term = pow(inv, m, mod_val)
        total = (total + term) % mod_val
    return total

def power_sum(q, m, mod_val):
    """
    Compute sum_{j=0}^{q-1} j^m mod mod_val
    Fast paths prevent O(q) loops for massive q at shallow depths.
    """
    if m == 0:
        return q % mod_val
    if m == 1:
        return (q * (q - 1) // 2) % mod_val
    if m == 2:
        return (q * (q - 1) * (2 * q - 1) // 6) % mod_val
    if m == 3:
        res = (q * (q - 1) // 2) % mod_val
        return (res * res) % mod_val
    
    # Fallback for higher m (only triggered when q is small)
    total = 0
    for j in range(q):
        total = (total + pow(j, m, mod_val)) % mod_val
    return total

def compute_carry(q, depth, mod_val):
    """Compute the exact p-adic carry S(q) using the Taylor expansion"""
    S_q = 0
    for m in range(MAX_DEPTH - depth):
        p_pow = p ** m
        inv_sum = sum_inverse_powers(m + 1, mod_val)
        pow_sum = power_sum(q, m, mod_val)
        
        term = (p_pow * inv_sum) % mod_val
        term = (term * pow_sum) % mod_val
        
        if m % 2 != 0:  # Handle the (-1)^m alternating sign
            term = (mod_val - term) % mod_val
            
        S_q = (S_q + term) % mod_val
    return S_q

def dfs(q, V_q, level):
    global max_J, max_M
    
    if level >= MAX_DEPTH:
        return
        
    current_mod = MOD[MAX_DEPTH - level]
    
    # Compute the EXACT base value including the Taylor series carry
    V_next_base = V_q // p
    carry = compute_carry(q, level, current_mod)
    V_next_base = (V_next_base + carry) % current_mod
    
    sum_r = 0
    
    for r in range(p):
        if r > 0:
            target = p * q + r
            sum_r = (sum_r + mod_inverse(target, current_mod)) % current_mod
            
        V_pq_r = (V_next_base + sum_r) % current_mod
        n = p * q + r
        
        if n > 0:
            if n > max_M:
                max_M = n
                
            if V_pq_r % p == 0:
                if n > max_J:
                    max_J = n
                dfs(n, V_pq_r, level + 1)

if __name__ == '__main__':
    print(f"Executing Python Taylor DFS for p = {p}...")
    dfs(0, 0, 0)
    print(f"Max n in J_p (Numerator divisible by p): {max_J}")
    print(f"Max n for PE 541 (Denominator NOT divisible by p): {max_M}")
