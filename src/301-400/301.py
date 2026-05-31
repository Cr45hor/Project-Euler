for n in range(1, 2**6):
    if n ^ (n * 2) ^ (n * 3) == 0:
        print(n, end=' ')