from numpy.polynomial import Polynomial

peter_dices = Polynomial([0] + [1/4] * 4)
colin_dices = Polynomial([0] + [1/6] * 6)

peter_sum = peter_dices ** 9
colin_sum = colin_dices ** 6

assert peter_sum.coef.size == colin_sum.coef.size
max_sum = peter_sum.coef.size - 1
win_prob = 0

for i in range(max_sum + 1):
    for j in range(i):
        win_prob += peter_sum.coef[i] * colin_sum.coef[j]

print(f"{win_prob:.7f}")

