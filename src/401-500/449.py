import numpy as np
from scipy.integrate import quad
from sympy import pi, symbols, sin, cos, sqrt, diff, lambdify

def V(a, b):
    return (4 * a * a * b * np.pi / 3)

def coat_fast(a, b, offset_val):
    t = symbols('t')

    # 1. Define the symbolic expressions
    sq_dis = sqrt(sin(t)**2 * a**2 + cos(t)**2 * b**2) # type: ignore
    x = cos(t) * (a + b * offset_val / sq_dis)
    z = sin(t) * (b + a * offset_val / sq_dis)

    # 2. Let SymPy calculate the derivative (this is instant)
    dz_dt = diff(z, t)
    
    # 3. Construct the full integrand expression
    integrand_expr = np.pi * 2 * x**2 * dz_dt

    # 4. Convert the SymPy expression to a fast numerical Python function
    # 'lambdify' takes the variable (t), the expression, and the backend ('numpy')
    integrand_func = lambdify(t, integrand_expr, 'numpy')

    # 5. Numerically integrate using SciPy (this takes milliseconds)
    # quad returns the result and the estimated error; we just want the result [0]
    V_tot, error = quad(integrand_func, 0, np.pi/2)
    
    V_inside = V(a, b)
    V_coat = V_tot - V_inside

    return V_coat

if __name__ == "__main__":
    a = 3
    b = 1
    offset = 1
    
    print(f"Volume of coat: {coat_fast(a, b, offset):.8f}")