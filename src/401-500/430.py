import sys

def W(i: int, n: int) -> float:
    assert 1 <= i <= n
    return ((i - 1) ** 2 + (n - i) ** 2) / (n ** 2)

def P(i: int, n: int, m: int) -> float:
    assert 1 <= i <= n
    return (1 + (1 - W(i, n) * 2) ** m) / 2

def E(n: int, m: int) -> float:
    # return sum(P(i, n, m) for i in range(1, n + 1))

    # Work with floats for performance and to avoid huge integers
    n_float = float(n)
    # c = (2n - 1) / n^2
    c = (2.0 * n_float - 1.0) / (n_float * n_float)
    
    # Compute J = ∫_0^1 (v^2 - c)^m dv via the binomial series.
    # The series converges extremely fast because c ≈ 2e-10.
    J = 1.0 / (2.0 * m + 1.0)
    term = J
    for t in range(1, m + 1):
        # ratio = A_t / A_{t-1}
        ratio = -c * (m - t + 1) / t * (2*m - 2*t + 3) / (2*m - 2*t + 1)
        term *= ratio
        if term == 0.0:
            break
        J += term
    
    # f'(1) = 2*m * (1 - c)^{m-1}
    f_prime_1 = 2.0 * m * ((1.0 - c) ** (m - 1))
    
    half_n = n_float / 2.0
    # Euler–Maclaurin corrected midpoint sum
    E = half_n + half_n * J - f_prime_1 / (12.0 * n_float)
    return E

if __name__ == '__main__':
    N = 10**10
    M = 4000
    print(E(N, M))