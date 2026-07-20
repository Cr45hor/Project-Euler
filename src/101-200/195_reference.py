import math

def pe195(N: int) -> int:
    # Limit for Family 1: (2N) / sqrt(3)
    limit_f1 = 2 * N / (3 ** 0.5)
    
    # Limit for Family 2: (6N) / sqrt(3)
    limit_f2 = 6 * N / (3 ** 0.5)
    
    cnt = 0

    # m(n-m) <= limit_f2 is the absolute maximum bound.
    # We substitute x = n - m.
    for m in range(1, int(limit_f2) + 1):
        # Your constraint 0 < m < n < 2m translates to 0 < x < m
        for x in range(1, m):
            
            # Since x increases, m*x will only get larger. 
            if m * x > limit_f2:
                break
                
            # Must be primitive
            if math.gcd(m, x) == 1:
                
                # Mod 3 Check 
                if (m - x) % 3 != 0:
                    # Family 1
                    if m * x <= limit_f1:
                        cnt += int(limit_f1 / (m * x))
                else:
                    # Family 2 (The sides were divided by 3)
                    if m * x <= limit_f2:
                        cnt += int(limit_f2 / (m * x))
                        
    return cnt

print(pe195(1053779))