n = 45
cnt = 0
k = 0
pow2 = 1

while cnt < n:
    k += 1
    pow2 *= 2
    prefix_pow2 = pow2
    while prefix_pow2 >= 1000:
        prefix_pow2 //= 10
    if prefix_pow2 == 123:
        # print(k, 2**k)
        cnt += 1

print(k)