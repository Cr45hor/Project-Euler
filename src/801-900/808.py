import bisect

N = 10**8
is_prime = [1] * (N + 1)
primes = []

def sieve():
    is_prime[0] = is_prime[1] = 0
    for i in range(4, N, 2):
        is_prime[i] = 0
    for i in range(3, int(N**0.5) + 1, 2):
        if is_prime[i]:
            for j in range(i * i, N, 2 * i):
                is_prime[j] = 0
    for i in range(2, N):
        if is_prime[i]:
            primes.append(i)

def rev(n: int) -> int:
    return int(str(n)[::-1])

def is_palindrome(n: int) -> bool:
    return n == rev(n)

def binary_search(arr, x):
    i = bisect.bisect_left(arr, x)
    if i != len(arr) and arr[i] == x:
        return i  # Found at index i
    return -1     # Not found

if __name__ == "__main__":
    sieve()

    prime_square = [p**2 for p in primes]

    cnt = 0
    sum = 0
    rps = []
    for p2 in prime_square:
        if not is_palindrome(p2) and binary_search(prime_square, rev(p2)) != -1:
            cnt += 1
            sum += p2
            rps.append(p2)
    print(cnt)
    print(sum)
    print(rps)