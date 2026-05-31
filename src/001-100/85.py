def T(n: int) -> int:
    return n * (n + 1) // 2

SOLUTION = 2 * (10 ** 6)
N = 2000
T_list = [(T(i), i) for i in range(1, N)]
counting_list = [(Ti * Tj, i * j) for Ti, i in T_list for Tj, j in T_list]
print(min(counting_list, key=lambda x: abs(x[0] - SOLUTION))[1])