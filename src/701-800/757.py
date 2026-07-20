from typing import List

def gen(n: int) -> List[int]:
    res: List[int] = []
    cap = int(n**0.5)
    for d in range(1, cap + 1):
        if n % d == 0:
            res.append(d + n // d)
    res.sort()
    return res

def check(n: int) -> bool:
    L = gen(n)
    for i in range(len(L) - 1):
        if L[i + 1] - L[i] == 1:
            return True
    return False

def S(N: int) -> int:
    cap = int(N**0.5)
    assert cap**2 <= N
    factor = [n * (n + 1) for n in range(1, cap + 1) if n * (n + 1) <= N]
    
    bipronics: set[int] = set()

    for i in range(len(factor)):
        for j in range(i, len(factor)):
            if factor[i] * factor[j] > N:
                break
            bipronics.add(factor[i] * factor[j])
    return len(bipronics)

if __name__ == "__main__":
    N = 10**14
    print(S(N))