import math

p = 1009
q = 3643
n = p * q
phi = (p - 1) * (q - 1)

sum = 0
cnt_min = n
for e in range(2, phi):
    if math.gcd(e, phi) != 1:
        continue
    cnt = (1 + math.gcd(e - 1, p - 1)) * (1 + math.gcd(e - 1, q - 1))
    if cnt < cnt_min:
        cnt_min = cnt
        sum = e
    elif cnt == cnt_min:
        sum += e
print(sum)