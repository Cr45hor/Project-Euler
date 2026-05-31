pentagoner = set(n * ((3 * n) - 1) // 2 for n in range(2, 4000))
for i in pentagoner:
    for b in pentagoner:
        if i + b in pentagoner:
                if i + 2*b in pentagoner:
                    print(i)