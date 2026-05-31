from fractions import Fraction

def get_convergent(partial_denoms: list[int]) -> Fraction:
    n = len(partial_denoms)
    frac = Fraction(partial_denoms[-1])
    for i in range(n - 2, -1, -1):
        frac = 1 / frac + partial_denoms[i]
    return frac

partial_denoms = [2]
for k in range(1, 100):
    partial_denoms += [1, 2 * k, 1]

N = 100
convergent = get_convergent(partial_denoms[:N])
num = convergent.numerator
print(sum(int(digit) for digit in str(num)))