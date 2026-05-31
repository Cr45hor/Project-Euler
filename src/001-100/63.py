def check(d, n):
    return len(str(d**n)) == n

print(sum(check(d, n) for d in range(1, 10) for n in range(1, 40)))