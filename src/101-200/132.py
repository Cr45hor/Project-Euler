N = 2 * 10**5
is_prime = [True] * N
primes = []

def sieve():
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(N**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, N, i):
                is_prime[j] = False
    for i in range(2, N):
        if is_prime[i]:
            primes.append(i)

K = 10**9

if __name__ == "__main__":
    sieve()

    primes_factors = []
    for p in primes:
        if p == 2 or p == 5:
            continue
        if p == 3:
            if K % 3 == 0:
                primes_factors.append(p)
        elif pow(10, K, p) == 1:
            primes_factors.append(p)

    if len(primes_factors) >= 40:
        print(sum(primes_factors[:40]))

