from sympy import symbols, integrate, sqrt, solve, pi

L = 1 - pi/4
THRESHOLD = 0.001

def f(n: int) -> float:
    x = symbols('x')

    # Line L: y = (1/n) * x
    # Circle: (x - 1)^2 + (y - 1)^2 = 1
    # Solve for intersection points
    eq = (x - 1)**2 + ((1/n) * x - 1)**2 - 1
    sol = solve(eq, x)
    P_x, _ = sorted(sol)
    P_y = (1/n) * P_x

    # Calculate the ratio
    area = P_x * P_y / 2 + (1 - P_x) - integrate(sqrt(2*x - x**2), (x, P_x, 1))
    ratio = area / L
    return ratio.evalf()

if __name__ == "__main__":
    l = 1
    r = 4096
    assert f(1) > THRESHOLD and f(4096) < THRESHOLD

    while l < r:
        mid = (l + r) // 2
        if f(mid) < THRESHOLD:
            r = mid
        else:
            l = mid + 1
    print(l, f(l))