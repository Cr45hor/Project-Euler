def decimal_digit(a, n):
    # return divmod(10**n, a)[0] % 10
    x = pow(10, n, 10 * a)
    r = pow(10, n, a)
    return ((x - r) % (10 * a)) // a

N = 10**7
print(sum(decimal_digit(i, N) for i in range(2, N + 1)))