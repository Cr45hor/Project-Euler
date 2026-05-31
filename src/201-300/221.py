def compute_optimized():
    # 1. Sieve primes up to a safe limit (200,000 is plenty to find the 150,000th integer)
    MAX_PRIME = 200000
    is_prime = [True] * MAX_PRIME
    primes = [2]
    
    for i in range(3, MAX_PRIME, 2):
        if is_prime[i]:
            # Mathematical shortcut: odd prime factors of p^2 + 1 are ALWAYS 1 (mod 4)
            if i % 4 == 1: 
                primes.append(i)
            for j in range(i * i, MAX_PRIME, i * 2):
                is_prime[j] = False

    def get_valid_divisors(n, max_val):
        """Returns divisors of n that are <= max_val using prime factorization."""
        factors = []
        temp = n
        for q in primes:
            if q * q > temp:
                break
            if temp % q == 0:
                count = 0
                while temp % q == 0:
                    count += 1
                    temp //= q
                factors.append((q, count))
                
        if temp > 1:
            factors.append((temp, 1))
            
        # Reconstruct divisors from prime factors
        divs = [1]
        for prime, count in factors:
            divs = [d * (prime ** power) for d in divs for power in range(count + 1)]
            
        return [d for d in divs if d <= max_val]

    # Use a set to prevent duplicate Alexandrian integers
    alexandrian_integers = set()
    p = 1
    
    target_index = 149999
    max_p = float('inf')
    
    # 2. Dynamically bound p based on the smallest mathematically guaranteed value
    while p <= max_p:
        n = p * p + 1
        for k in get_valid_divisors(n, p):
            A = p * (p + k) * (p + n // k)
            alexandrian_integers.add(A)
            
        # Once we have a healthy buffer, establish a strict mathematical ceiling for p
        if len(alexandrian_integers) > target_index + 5000 and max_p == float('inf'):
            current_target = sorted(alexandrian_integers)[target_index]
            # Since A = p(p+k)(p + (p^2+1)/k), the minimum possible A for a given p is strictly > p^3
            # Therefore, we can safely stop iterating entirely once p^3 > current_target
            max_p = int(current_target ** (1/3.0)) + 1
            
        p += 1
        
    return sorted(alexandrian_integers)[target_index]

print(compute_optimized())