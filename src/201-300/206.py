import itertools

aim = "1_2_3_4_5_6_7_8_9_0"

L = 1000000000
R = 1500000000

for n in range(L, R + 1, 10):
    mask = str(n * n)[0:17:2]
    if mask == "123456789":
        print(n)
        break