def solve_f_n(n: int, k: int = 2000, mod: int = 20092010) -> int:
    """
    Computes f_n modulo (10^9 + 7) in O(k^2 log n) time using Fiduccia's algorithm.
    """
    if n < k:
        return 1 % mod

    def multiply_and_reduce(A: list[int], B: list[int]) -> list[int]:
        # Array to hold the intermediate polynomial of degree up to 2k - 2
        C = [0] * (2 * k)
        
        # 1. Polynomial Multiplication
        # We DO NOT apply modulo here. Python handles the ~70-bit integers natively,
        # which saves 4 million modulo operations per multiplication step.
        for i, a in enumerate(A):
            if not a:
                continue
            for j, b in enumerate(B):
                if not b:
                    continue
                C[i + j] += a * b
                
        # 2. Polynomial Reduction modulo (x^2000 - x - 1)
        # Identity: x^m = x^{m-1999} + x^{m-2000}
        # We reduce from the highest degree down to k (2000).
        for i in range(2 * k - 2, k - 1, -1):
            if C[i]:
                C[i - 1999] += C[i]
                C[i - 2000] += C[i]
                
        # 3. Apply the modulo once per element on the reduced polynomial
        return [x % mod for x in C[:k]]

    # Initialize the result as the identity polynomial: x^0 = 1
    res = [0] * k
    res[0] = 1
    
    # Initialize the base polynomial to multiply: x^1
    base = [0] * k
    base[1] = 1
    
    # Binary exponentiation to compute x^n mod P(x)
    while n > 0:
        if n % 2 == 1:
            res = multiply_and_reduce(res, base)
        base = multiply_and_reduce(base, base)
        n //= 2
        
    # Because our initial conditions are f_i = 1 for all 0 <= i < 2000,
    # the final value f_n is simply the sum of the coefficients:
    # f_n = sum(c_i * f_i) = sum(c_i * 1)
    return sum(res) % mod

if __name__ == "__main__":
    n_target = 10**18
    print(f"Computing f_n for n = {n_target}...")
    
    result = solve_f_n(n_target)
    
    # Using exact integer output to avoid scientific notation truncation
    print(f"Result: {result}")