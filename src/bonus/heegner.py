from decimal import Decimal, getcontext, ROUND_CEILING

# Configuration
PRECISION = 150
getcontext().prec = PRECISION
PI = Decimal('3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679')

def cos_decimal(x: Decimal) -> Decimal:
    """High-precision cosine using Taylor series."""
    getcontext().prec += 2
    i, lasts, s, fact, num, sign = 0, 0, 1, 1, 1, 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i - 1)
        num *= x * x
        sign *= -1
        s += (num / fact) * sign
    getcontext().prec -= 2
    return +s

def get_target_value(n: int) -> Decimal:
    """Calculates cosh(sqrt(|n|) * pi) for n < 0 or cos(sqrt(n) * pi) for n > 0."""
    # Using the built-in abs() on the Decimal object
    arg = abs(Decimal(n)).sqrt() * PI
    
    if n < 0:
        return (arg.exp() + (-arg).exp()) / 2
    return cos_decimal(arg)

def find_minimal_distance(limit: int):
    results = []
    
    for n in range(-limit + 1, limit):
        # Skip perfect squares
        root = int(abs(n)**0.5)
        if root * root == abs(n):
            continue

        val = get_target_value(n)
        
        # distance = |ceil(val) - val|
        ceiling = val.to_integral_value(rounding=ROUND_CEILING)
        dist = abs(ceiling - val)
        
        results.append((n, dist))

    return min(results, key=lambda x: x[1])

if __name__ == "__main__":
    best_n, min_dist = find_minimal_distance(1000)
    print(f"Result: n={best_n}, distance={min_dist}")