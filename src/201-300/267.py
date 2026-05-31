import math

def solve_fast(N=1000, target=10**9):
    target_log = math.log10(target)
    min_k = N + 1
    
    # We only check k values where the optimal p is positive
    # Since p = (3k - N) / 2N, p > 0 requires k > N / 3
    start_k = math.ceil(N / 3)
    
    # 1. Find the minimum k required to reach the target wealth
    for k in range(start_k, N + 1):
        # Calculate the exact optimal p for this specific k
        p = (3 * k - N) / (2 * N)
        
        # Use log10 to evaluate wealth to avoid large number overflow
        # V = (1 + 2p)^k * (1 - p)^(N-k)
        log_V = k * math.log10(1 + 2 * p) + (N - k) * math.log10(1 - p)
        
        # Check if this optimal p gets us to the billion dollar target
        if log_V >= target_log:
            min_k = k
            break
            
    # 2. Calculate the exact probability using Python's native large integer math
    # math.comb replaces sympy.binomial, and (1 << N) replaces Decimal(2)**N
    ways_to_win = sum(math.comb(N, k) for k in range(min_k, N + 1))
    total_outcomes = 1 << N
    
    probability = ways_to_win / total_outcomes
    return probability, min_k

if __name__ == "__main__":
    prob, required_heads = solve_fast()
    print(f"Minimum heads needed: {required_heads}")
    print(f"Probability: {prob:.12f}")