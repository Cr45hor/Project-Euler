MOD = 10**9 + 7

# ==========================================
# STAGE 1: The Generator (Ground Truth)
# ==========================================
def generate_terms(limit):
    """
    Uses your O(N^2) DP to generate the first 'limit' terms of A(N).
    We only need about 2000 terms to confidently guess the formula!
    """
    F = [0, 1]
    for _ in range(limit + 5):
        F.append((F[-1] + F[-2]) % MOD)
        
    invFact = [1] * (2 * limit + 5)
    fact = [1] * (2 * limit + 5)
    for i in range(1, len(fact)):
        fact[i] = (fact[i - 1] * i) % MOD
    invFact[-1] = pow(fact[-1], MOD - 2, MOD)
    for i in range(len(fact) - 2, 0, -1):
        invFact[i] = (invFact[i + 1] * (i + 1)) % MOD

    def C(n):
        return fact[n * 2 + 1] * invFact[n] % MOD * invFact[n + 1] % MOD * pow(n * 2 + 1, MOD - 2, MOD) % MOD

    dp0 = [0] * (limit + 1)
    dp1 = [0] * (limit + 1)
    dp0[0], dp1[1] = F[0], F[1]
    
    A0 = [(C(i) - F[i] * C(i)) % MOD for i in range(limit + 1)]
    A1 = [(C(i) - F[i + 2] * C(i)) % MOD for i in range(limit + 1)]
    B  = [(F[i + 1] * C(i)) % MOD for i in range(limit + 1)]
    
    for r in range(1, limit + 1):
        if r >= 2:
            sum_A1 = sum(dp1[i] * A1[r - 1 - i] for i in range(1, r)) % MOD
            sum_B0 = sum(dp0[i] * B[r - 2 - i] for i in range(0, r - 1)) % MOD
            dp1[r] = (sum_A1 - sum_B0) % MOD
            
        sum_A0 = sum(dp0[i] * A0[r - 1 - i] for i in range(0, r)) % MOD
        sum_B1 = sum(dp1[i] * B[r - i] for i in range(1, r + 1)) % MOD
        dp0[r] = (sum_A0 - sum_B1) % MOD
        
    return dp0

# ==========================================
# STAGE 2: The Oracle (Gaussian Guesser)
# ==========================================
def find_recurrence(seq, D, d):
    """
    Finds a recurrence: sum_{i=0}^D P_i(n) * A_{n-i} = 0
    where P_i(n) is a polynomial of degree up to 'd'.
    Uses Gaussian Elimination over GF(MOD).
    """
    cols = (D + 1) * (d + 1)
    rows = len(seq) - D
    
    if rows < cols:
        raise ValueError("Not enough terms generated to solve the matrix!")

    # Build the matrix
    matrix = [[0] * cols for _ in range(rows)]
    for r in range(rows):
        n = r + D  # The current index we are evaluating
        for i in range(D + 1):
            for j in range(d + 1):
                col_idx = i * (d + 1) + j
                # Term is: (n^j) * A[n-i]
                matrix[r][col_idx] = (pow(n, j, MOD) * seq[n - i]) % MOD

    # Standard Gaussian Elimination over GF(MOD)
    row = 0
    free_vars = []
    pivot_cols = []
    
    for col in range(cols):
        # Find pivot
        pivot = -1
        for i in range(row, rows):
            if matrix[i][col] != 0:
                pivot = i
                break
                
        if pivot == -1:
            free_vars.append(col)
            continue
            
        # Swap rows
        matrix[row], matrix[pivot] = matrix[pivot], matrix[row]
        pivot_cols.append(col)
        
        # Normalize pivot row
        inv_val = pow(matrix[row][col], MOD - 2, MOD)
        for j in range(col, cols):
            matrix[row][j] = (matrix[row][j] * inv_val) % MOD
            
        # Eliminate below
        for i in range(row + 1, rows):
            if matrix[i][col] != 0:
                factor = matrix[i][col]
                for j in range(col, cols):
                    matrix[i][j] = (matrix[i][j] - factor * matrix[row][j]) % MOD
        row += 1

    # If there are no free variables, we failed to find a valid recurrence
    if not free_vars:
        return None 
        
    # Back-substitute to find the null space vector
    # We set the last free variable to 1, others to 0
    solution = [0] * cols
    target_free = free_vars[-1]
    solution[target_free] = 1
    
    for r in range(len(pivot_cols) - 1, -1, -1):
        p_col = pivot_cols[r]
        val = 0
        for j in range(p_col + 1, cols):
            val = (val + matrix[r][j] * solution[j]) % MOD
        solution[p_col] = (-val) % MOD

    # Extract the polynomials P_i(n)
    polynomials = []
    for i in range(D + 1):
        poly = solution[i * (d + 1) : (i + 1) * (d + 1)]
        polynomials.append(poly)
        
    return polynomials

# ==========================================
# STAGE 3: The O(N) Engine
# ==========================================
def solve_ON(N, base_terms, polynomials):
    """
    Uses the discovered polynomials to evaluate A(N) in pure O(N) time.
    P_0(n)*A_n + P_1(n)*A_{n-1} + ... + P_D(n)*A_{n-D} = 0
    => A_n = - (1 / P_0(n)) * sum_{i=1}^D P_i(n)*A_{n-i}
    """
    D = len(polynomials) - 1
    d = len(polynomials[0]) - 1
    
    # We can just keep a sliding window of the last D terms
    window = base_terms[:D]
    
    def eval_poly(poly, n):
        res = 0
        n_pow = 1
        for coef in poly:
            res = (res + coef * n_pow) % MOD
            n_pow = (n_pow * n) % MOD
        return res

    for n in range(D, N + 1):
        # Evaluate P_0(n)
        P0 = eval_poly(polynomials[0], n)
        
        # If P0 is 0 modulo MOD, the recurrence breaks down at this specific point.
        # (Very rare for large primes, but handled by advanced guessers by scaling).
        if P0 == 0:
            raise Exception(f"Leading polynomial evaluated to 0 at n={n}")
            
        inv_P0 = pow(P0, MOD - 2, MOD)
        
        # Compute the sum of the rest of the terms
        rhs = 0
        for i in range(1, D + 1):
            Pi = eval_poly(polynomials[i], n)
            # window[D - i] is A_{n-i} because we shift the window
            rhs = (rhs + Pi * window[D - i]) % MOD
            
        # A_n = - (RHS) / P0
        A_n = (-rhs * inv_P0) % MOD
        
        # Shift the window forward
        window.pop(0)
        window.append(A_n)
        
    return window[-1]

# ==========================================
# EXECUTION
# ==========================================
if __name__ == "__main__":
    print("1. Generating Ground Truth (2000 terms)...")
    base_terms = generate_terms(2000)
    
    print("2. Firing up the Oracle (Gaussian Elimination)...")
    polys = find_recurrence(base_terms, D=30, d=10)
    
    if polys:
        print("-> Recurrence Found!")
        print(polys)
        
        target = 10**7
        print(f"3. Booting O(N) Engine to calculate A({target})...")
        
        # This will chew through 10,000,000 in a few seconds
        ans = solve_ON(target, base_terms, polys)
        print(f"\nFinal Answer for A({target}): {ans}")
    else:
        print("-> Failed to find recurrence. Increase D or d, or generate more terms.")