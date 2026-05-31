def solve_pe412(m, n, p):
    N = m**2 - n**2
    
    # 1. Compute N! mod p using Wilson's Theorem
    # Since p is prime and N is close to p, N! = (p-1)! / ((N+1) * ... * (p-1)) mod p
    # By Wilson's Theorem: (p-1)! = -1 mod p
    num = -1
    denom = 1
    for i in range(N + 1, p):
        denom = (denom * i) % p
    
    n_fact = (num * pow(denom, p - 2, p)) % p
    
    # 2. Function to compute the hook length product for the rectangular arms (Regions B and C)
    def prod_B_C_hooks(R, C, mod):
        fact = 1
        facts = [1] * (R + C + 1)
        for i in range(1, R + C + 1):
            fact = (fact * i) % mod
            facts[i] = fact
            
        num_prod = 1
        for i in range(1, R + 1):
            num_prod = (num_prod * facts[i + C - 1]) % mod
            
        den_prod = 1
        for i in range(1, R + 1):
            den_prod = (den_prod * facts[i - 1]) % mod
            
        return (num_prod * pow(den_prod, mod - 2, mod)) % mod

    # Product for Region B (top-right arm of the L-shape)
    prod_B = prod_B_C_hooks(m - n, n, p)
    # Product for Region C (bottom-left arm of the L-shape)
    prod_C = prod_B_C_hooks(n, m - n, p)
    
    # 3. Product for Region A (the central square of the L-shape)
    prod_A = 1
    R = m - n
    # Iterate through the diagonals of the square to group identical hook lengths
    for k in range(2, 2 * R + 1):
        count = min(k - 1, 2 * R - k + 1)
        val = 2 * m - k + 1
        prod_A = (prod_A * pow(val, count, p)) % p
        
    total_hook_prod = (prod_A * prod_B * prod_C) % p
    
    # 4. Final calculation: LC(m, n) = N! / total_hook_prod mod p
    ans = (n_fact * pow(total_hook_prod, p - 2, p)) % p
    return ans

if __name__ == "__main__":
    m = 10000
    n = 5000
    p = 76543217
    
    result = solve_pe412(m, n, p)
    print(f"LC({m}, {n}) mod {p} = {result}")