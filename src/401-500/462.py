import math
from decimal import Decimal, getcontext

def compute_F(N):
    # Use the decimal module to format the huge integer accurately
    getcontext().prec = 50 

    # 1. Build the partition (row lengths)
    lambdas = []
    b = 0
    while True:
        pow3 = 3**b
        if pow3 > N:
            break
        
        # Count how many powers of 2 we can multiply by 3^b without exceeding N
        val = N // pow3
        count = 0
        while val >= 1:
            count += 1
            val //= 2
        
        lambdas.append(count)
        b += 1

    n = sum(lambdas)
    denominator = 1
    
    # 2. Compute the product of all hook lengths
    for r in range(len(lambdas)):
        for c in range(lambdas[r]):
            # Cells to the right in the same row
            right = lambdas[r] - 1 - c
            
            # Cells below in the same column
            below = 0
            for r_prime in range(r + 1, len(lambdas)):
                if lambdas[r_prime] > c:
                    below += 1
                else:
                    break
                    
            hook = right + below + 1
            denominator *= hook
            
    # 3. Apply the Hook Length Formula
    ans = math.factorial(n) // denominator
    
    # Format to scientific notation with 10 decimal places
    return f"{Decimal(ans):.10e}"

# Compute for N = 10^18
# print(f"F(10^18) = {compute_F(10**18)}")
print(compute_F(10**18))