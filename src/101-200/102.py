def area(poly: list[tuple[int, int]]):
    n = len(poly)
    S = sum(poly[i][0] * poly[i + 1][1] - poly[i + 1][0] * poly[i][1] for i in range(-1, n - 1))
    return abs(S) / 2

with open("0102_triangles.txt") as f:
    triangles = [list(map(int, line.split(","))) for line in f]
    cnt = 0
    for triangle in triangles:
        A = (triangle[0], triangle[1])
        B = (triangle[2], triangle[3])
        C = (triangle[4], triangle[5])
        S_OAB = area([(0, 0), A, B])
        S_OBC = area([(0, 0), B, C])
        S_OCA = area([(0, 0), C, A])
        S_ABC = area([A, B, C])
        if S_OAB + S_OBC + S_OCA == S_ABC:
            cnt += 1
    print(cnt)
    