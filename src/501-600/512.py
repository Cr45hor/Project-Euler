N = 5 * 10**8
spf = [1] * (N + 1)

def sieve():
    for i in range(2, N + 1):
        if spf[i] == 1:
            spf[i] = i
            for j in range(i * i, N + 1, i):
                if spf[j] == 1:
                    spf[j] = i

def phi(n: int) -> int:
    result = n
    while n > 1:
        p = spf[n]
        result -= result // p
        while n % p == 0:
            n //= p
    return result

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        gcd, x1, y1 = extended_gcd(b % a, a)
        x = y1 - (b // a) * x1
        y = x1
        return gcd, x, y

def invmod(a: int, m: int) -> int:
    gcd, x, y = extended_gcd(a, m)
    if gcd != 1:
        raise ValueError("Modular inverse does not exist")
    assert a * x + m * y == 1
    return ((x % m) + m) % m

def f(n: int) -> int:
    if n == 1:
        return 1
    return phi(n) * ((pow(n, n, n + 1) - 1) * invmod(n - 1, n + 1) % (n + 1) if n % 2 == 0 else 1)

def g(n: int) -> int:
    return sum(f(i) for i in range(1, n + 1))

if __name__ == "__main__":
    sieve()
    print(g(N))