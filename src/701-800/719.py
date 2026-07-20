def check(n: int, target: int) -> bool:
    s = str(n)
    L = len(s)

    prefix = [0] + [int(s[:i]) for i in range(1, L + 1)]
    assert len(prefix) == L + 1

    def v(l: int, r: int) -> int:
        # Return the value of the substring s[l:r] as an integer
        return prefix[r] - prefix[l] * 10**(r - l)
    
    partition = [[0]]
    for r in range(1, L + 1):
        next_partition = []
        for l in range(r):
            # s[l:r] + partition[l]
            add = v(l, r)
            for prev in partition[l]:
                next_partition.append(prev + add)
        # print(f"r: {r}, next_partition: {next_partition}")
        partition.append(next_partition)
    
    assert len(partition) == L + 1
    return target in partition[L]

def T(N: int) -> int:
    cap = int(N**0.5)
    assert cap**2 <= N

    sum = 0
    for n in range(9, cap + 1, 9):
        for modulo in [0, 1]:
            cand = n + modulo
            if cand > cap:
                break
            if check(nn := cand**2, cand):
                sum += nn
    return sum

if __name__ == "__main__":
    N = 10**12
    print(T(N))