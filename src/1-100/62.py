THRESHOLD = 5

cubic_perm = {}
x = 1
cnt = 0

while True:
    x_cubic = x ** 3
    x_cubic_str = "".join(sorted(str(x_cubic)))
    cubic_perm.setdefault(x_cubic_str, []).append(x)
    if len(cubic_perm[x_cubic_str]) == THRESHOLD:
        print(cubic_perm[x_cubic_str][0] ** 3)
        break

    x += 1
