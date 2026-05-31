def solve_pell_like(D, N, limit=12):
    solutions = []
    a, b = 1, 1
    for T in range(1, 50):
        a, b = (a * 3 + b * 5) // 2, (a + b * 3) // 2
        # print(f"A = {a}, B = {b}")
        if b % 2 == 0 and (a % 5 == 4 or a % 5 == 1):
            L = b // 2
            h = (a - 1) // 5 if a % 5 == 1 else (a + 1) // 5
            if L > h:
                print(f"  L = {L}, h = {h}, b = {int((L**2 - h**2)**0.5)}")
                solutions.append((L, h))
            
        if len(solutions) >= limit:
            break
    return solutions

print(sum(L for L, h in solve_pell_like(5, -4)[:12]))