import sympy as sp

# Pure Python 2x2 matrix multiplication using tuples (much faster than np.object)
def mat_mult(A, B, MOD):
    return (
        ((A[0][0]*B[0][0] + A[0][1]*B[1][0]) % MOD, (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % MOD),
        ((A[1][0]*B[0][0] + A[1][1]*B[1][0]) % MOD, (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % MOD)
    )

# Fast matrix exponentiation
def mat_pow(A, p, MOD):
    res = ((1, 0), (0, 1)) # Identity matrix
    base = A
    while p > 0:
        if p % 2 == 1:
            res = mat_mult(res, base, MOD)
        base = mat_mult(base, base, MOD)
        p //= 2
    return res

def solve():
    N = 10**5
    SEED = sp.nextprime(10**14)
    MOD = 1234567891011

    # 1. Generate primes
    A = [SEED]
    curr_prime = SEED
    for _ in range(1, N):
        curr_prime = sp.nextprime(curr_prime)
        A.append(curr_prime)

    # Note: For M = [[1, 1], [1, 0]], the value F(n) is exactly M^n[0][1]
    M = ((1, 1), (1, 0))
    
    # Base calculation for the first prime
    curr_M = mat_pow(M, A[0], MOD)
    total_sum = curr_M[0][1]

    # Cache for prime gaps
    memo_gaps = {}

    # 2. Incrementally calculate F(p) for the rest
    for i in range(1, N):
        gap = A[i] - A[i-1]
        
        # Calculate or fetch the matrix for this specific gap
        if gap not in memo_gaps:
            memo_gaps[gap] = mat_pow(M, gap, MOD)
            
        # Advance the current matrix state by the gap
        curr_M = mat_mult(curr_M, memo_gaps[gap], MOD)
        
        # Add F(p_i) to total
        total_sum = (total_sum + curr_M[0][1]) % MOD

    return total_sum

if __name__ == "__main__":
    print(solve())