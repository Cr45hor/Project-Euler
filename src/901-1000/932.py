N = 16

n = 4
sum = 0
while True:
    nn = n * n
    nn_str = str(nn)
    l = len(nn_str)
    if l > N:
        break

    if l % 2 == 0:
        a = int(nn_str[:l//2])
        b = int(nn_str[l//2:])
        if a > 0 and b > 0 and nn_str[l//2] != '0' and a + b == n:
            sum += nn
    else:
        a1 = int(nn_str[:l//2])
        b1 = int(nn_str[l//2:])
        a2 = int(nn_str[:l//2+1])
        b2 = int(nn_str[l//2+1:])
        if a1 > 0 and b1 > 0 and nn_str[l//2] != '0' and a1 + b1 == n:
            sum += nn
        elif a2 > 0 and b2 > 0 and nn_str[l//2+1] != '0' and a2 + b2 == n:
            sum += nn
    n += 1

print(sum)
