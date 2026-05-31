import math

THRESHOLD = 4_000_000
TARGET_P = 2 * THRESHOLD - 1
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

def solve():
    # We use your [1] * 15 configuration as the initial absolute upper bound
    min_n = math.prod(primes)
    
    def backtrack(idx: int, current_n: int, current_p: int, limit_e: int):
        nonlocal min_n
        
        # Base case: we hit the required threshold
        if current_p > TARGET_P:
            if current_n < min_n:
                min_n = current_n
            return
            
        # Base case: run out of available primes in the list
        if idx >= len(primes):
            return
            
        p = primes[idx]
        e = 1
        new_n = current_n * p
        
        # Try exponents from 1 up to the exponent of the previous prime (limit_e)
        while e <= limit_e:
            # Prune the branch if we exceed our best found minimum
            if new_n >= min_n:
                break
                
            new_p = current_p * (2 * e + 1)
            backtrack(idx + 1, new_n, new_p, e)
            
            e += 1
            new_n *= p

    # 60 is a safe upper bound for the exponent of 2, since 2^60 > min_n
    backtrack(0, 1, 1, 60)
    return min_n

if __name__ == "__main__":
    result = solve()
    print(result)