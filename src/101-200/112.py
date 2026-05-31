def check_non_decreasing(n):
    digits = [int(d) for d in str(n)]
    if len(digits) < 2:
        return True
    return all(digits[i] <= digits[i + 1] for i in range(len(digits) - 1))

def check_non_increasing(n):
    digits = [int(d) for d in str(n)]
    if len(digits) < 2:
        return True
    return all(digits[i] >= digits[i + 1] for i in range(len(digits) - 1))

def check_bouncy(n):
    return not check_non_decreasing(n) and not check_non_increasing(n)

cnt = 0
n = 0
while True:
    n += 1
    if check_bouncy(n):
        cnt += 1
    if cnt / n >= 0.99:
        print(n)
        break