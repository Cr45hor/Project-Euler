def generate_solutions():
    """
    Generates infinite solutions (k, n, m) for the equation:
    (n + 1)^2 + 4n^2 = m^2
    """
    # Base cases for k = 0 and k = 1
    n_prev, n_curr = 0, -1
    m_prev, m_curr = 1, 2
    
    # Yield the first two solutions
    yield 0, n_prev, m_prev
    yield 1, n_curr, m_curr
    
    k = 2
    while True:
        # Apply the linear recurrence relations
        n_next = -3 * n_curr - n_prev - 1
        m_next = 3 * m_curr - m_prev
        
        yield k, n_next, m_next
        
        # Shift values forward for the next iteration
        n_prev, n_curr = n_curr, n_next
        m_prev, m_curr = m_curr, m_next
        k += 1

if __name__ == "__main__":
    # Initialize the generator
    solutions = generate_solutions()
    
    # Set how many solutions you want to list
    num_solutions_to_find = 32
    
    print(f"{'k':<4} | {'n':<12} | {'m':<12} | {'Verification (LHS == RHS)'}")
    print("-" * 65)
    
    for _ in range(num_solutions_to_find):
        k, n, m = next(solutions)
        
        # Verify that the solution fits the original equation
        lhs = (n + 1)**2 + 4 * n**2
        rhs = m**2
        is_valid = (lhs == rhs)
        
        # Note: m can be ±m, we just print the positive version here
        print(f"{k:<4} | {n:<12} | {m:<12} | {is_valid} (Value: {rhs})")