from itertools import permutations

prime_list = [2, 3, 5, 7, 11, 13, 17]

cnt = 0
sum = 0
for perm in permutations('0123456789'):
    number = int(''.join(perm))
    substr_list = [int(''.join(perm[i:i + 3])) for i in range(1, 8)]
    divisibility_list = [(num % p == 0) for num, p in zip(substr_list, prime_list)]
    if (0 in divisibility_list) == False:
        cnt += 1
        sum += number
print(sum)