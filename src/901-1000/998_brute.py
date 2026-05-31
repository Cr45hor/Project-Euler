import numpy as np
from scipy.optimize import minimize_scalar
import math
import time

def get_min_bounding_square(a, b, c):
    """
    Calculates exact minimum bounding square side using Brent's optimization method.
    """
    x_A = (a**2 + c**2 - b**2) / (2 * a)
    y_A_sq = c**2 - x_A**2
    
    if y_A_sq <= 0:
        return 0.0 # Degenerate triangle
        
    y_A = math.sqrt(y_A_sq)
    
    Vx = np.array([0.0, a, x_A])
    Vy = np.array([0.0, 0.0, y_A])
    
    def bounding_square_size(theta):
        cos_t, sin_t = math.cos(theta), math.sin(theta)
        
        # Rotate coordinates
        X = Vx * cos_t - Vy * sin_t
        Y = Vx * sin_t + Vy * cos_t
        
        W = np.max(X) - np.min(X)
        H = np.max(Y) - np.min(Y)
        return max(W, H)

    # The max(W, H) function can have a sharp peak where W = H.
    # To avoid getting stuck in a false local minimum, we evaluate two chunks of the 90-degree quadrant.
    best_min = float('inf')
    for start_angle in [0, np.pi/4]:
        res = minimize_scalar(
            bounding_square_size, 
            bounds=(start_angle, start_angle + np.pi/4), 
            method='bounded',
            options={'xatol': 1e-9}
        )
        if res.fun < best_min:
            best_min = res.fun
            
    return best_min

def is_integer_bounding_square(a, b, c, tol=1e-5):
    min_sq = get_min_bounding_square(a, b, c)
    nearest_int = round(min_sq)
    
    if abs(min_sq - nearest_int) < tol:
        return True, nearest_int, min_sq
    return False, nearest_int, min_sq

def compute_T(n):
    print(f"--- Computing T({n}) ---")
    start_time = time.time()
    
    total_perimeter = 0
    max_side = int(n * math.sqrt(2)) + 1
    
    print(f"{'Triangle (a, b, c)':<20} | {'Square Size':<12} | {'Perimeter'}")
    print("-" * 55)
    
    for a in range(1, max_side + 1):
        for b in range(1, a + 1):
            for c in range(a - b + 1, b + 1):
                
                is_int, sq_size, _ = is_integer_bounding_square(a, b, c)
                
                if is_int and sq_size <= n:
                    p = a + b + c
                    total_perimeter += p
                    print(f"({a:>2}, {b:>2}, {c:>2})          | {sq_size:>12} | {p:>4}")

    elapsed = time.time() - start_time
    print("-" * 55)
    print(f"T({n}) = {total_perimeter}")
    print(f"Time taken: {elapsed:.3f} seconds\n")

if __name__ == "__main__":
    is_int, val, raw = is_integer_bounding_square(5, 4, 3)
    print(f"Triangle (3, 4, 5): Minimum Square = {raw:.5f} | Is Integer? {is_int}")
    
    is_int, val, raw = is_integer_bounding_square(13, 13, 10)
    print(f"Triangle (10, 13, 13): Minimum Square = {raw:.5f} | Is Integer? {is_int}\n")
    
    compute_T(40)