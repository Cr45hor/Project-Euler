import math
from fpylll import IntegerMatrix, LLL
from decimal import Decimal, getcontext

getcontext().prec = 200 
pi = Decimal("3.141592653589793238462643383279502884197169399375158209749445923078164062862089986280348253421170679")

def pe591(N: int = 10**13):
    C = Decimal(N**2)
    W = N

    total_sum = 0    
    for d in range(2, 100):
        if math.isqrt(d)**2 == d:
            continue
        sqrt_d = Decimal(d).sqrt()
        
        B = IntegerMatrix(3, 4)
        B[0, 0] = 1; B[0, 1] = 0; B[0, 2] = 0; B[0, 3] = int(C * Decimal(1))
        B[1, 0] = 0; B[1, 1] = 1; B[1, 2] = 0; B[1, 3] = int(C * sqrt_d)
        B[2, 0] = 0; B[2, 1] = 0; B[2, 2] = W; B[2, 3] = int(-C * pi)
        LLL.reduction(B)
        
        best = (Decimal('inf'), None, None)
        
        # Extract the target c-coefficients from the reduced basis
        c0 = B[0, 2] // W
        c1 = B[1, 2] // W
        c2 = B[2, 2] // W
        
        search_range = 50
        for x in range(-search_range, search_range + 1):
            for y in range(-search_range, search_range + 1):
                for z in range(-search_range, search_range + 1):
                    a = x*B[0,0] + y*B[1,0] + z*B[2,0]
                    b = x*B[0,1] + y*B[1,1] + z*B[2,1]
                    if abs(a) <= N and abs(b) <= N:
                        val = Decimal(a) + Decimal(b) * sqrt_d
                        err = abs(val - pi)
                        best = min(best, (err, a, b))
        
        if best[1] is not None:
            total_sum += abs(best[1])
            
    return total_sum

if __name__ == "__main__":
    print(pe591(10**18))