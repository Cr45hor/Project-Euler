import gmpy2
from itertools import permutations

# Select a subset of digits 1 to 9
# Then permute the selected digits and check if the resulting number is prime

pandigital_primes = []
for k in range(1, 10):
    for perm in permutations(range(1, k + 1)):
        number = int(''.join(str(d) for d in perm))
        if gmpy2.is_prime(number): # type: ignore
            pandigital_primes.append(number)

print(max(pandigital_primes))
        