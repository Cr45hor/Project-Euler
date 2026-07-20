from math import sqrt
import math

def Mobius(n):
    prime = [1] * (n + 1)
    mobius = [1] * (n + 1)

    for i in range(2, n + 1):
        if not prime[i]:
            continue
        mobius[i] = -1
        for j in range(2, n // i + 1):
            prime[i * j] = 0
            mobius[i * j] *= -1
        for j in range(1, n // (i * i) + 1):
            mobius[j * i * i] = 0
    return mobius

def efficient_square_free(N, Imax):
    D = int(sqrt(N / Imax))
    mobius = Mobius(D)

    # compute S1
    s1 = 0
    for i in range(1, D + 1):
        s1 += mobius[i] * (N // (i * i))

    # compute M(d), d = 1, ..., D
    M_list = [0]
    M = 0
    for m in mobius[1:]:
        M += m
        M_list.append(M)

    # compute M(sqrt(n / i)), i = Imax - 1, ..., 1
    Mxi_list = []
    Mxi_sum = 0
    for i in range(Imax - 1, 0, -1):
        Mxi = 1
        xi = int(sqrt(N // i))

        sqd = int(sqrt(xi))
        # sqd < D <= xi
        for j in range(1, xi // (sqd + 1) + 1):
            Mxi -= (xi // j - xi // (j + 1)) * M_list[j]

        for j in range(2, sqd + 1):
            if xi // j <= D:
                Mxi -= M_list[xi // j]
            else:
                Mxi -= Mxi_list[Imax - j * j * i - 1]

        Mxi_list.append(Mxi)
        Mxi_sum += Mxi

    # compute S2
    s2 = Mxi_sum - (Imax - 1) * M_list[-1]
    return s1 + s2

def g(N: int):
    max_sqrt = math.isqrt(N)
    sum = 0
    for g in range(1, max_sqrt + 1):
        M = N // (g * g)
        sum += g**2 * efficient_square_free(M, int(M**0.2))
    return sum

print(g(10**14))