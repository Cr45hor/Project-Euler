res = []

for base in range(2, 100):
    for expo in range(2, 50):
        n = base ** expo
        if sum(int(d) for d in str(n)) == base:
            print(f"Found: {n} (base={base}, expo={expo})")
            res.append(n)

sorted(res)
# print(res[29])
for i, val in enumerate(sorted(res)):
    if i < 30:
        print(f"{i+1}: {val}")